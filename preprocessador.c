//Aluno: Edmar Silva Jeremias e Igor Rimijo Pereira

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tamanho máximo para uma linha e número máximo de #define
#define MAX_LINE 1024
#define MAX_DEFINE 100

// Estrutura para armazenar um #define (nome e valor)
typedef struct {
    char chaves[128];
    char valor[512];
} Define;

Define defines[MAX_DEFINE]; // Vetor global para guardar os defines
int contador_defines = 0;       // Contador de defines

// -------------------------------------------------------------------
// Função: remover_comentarios
// Objetivo: Ler o arquivo 'input' caractere a caractere, removendo os comentários e escrevendo o resultado em 'output'.
// -------------------------------------------------------------------
void remover_comentarios(FILE *input, FILE *output) {
    int c, next;
    while ((c = fgetc(input)) != EOF) { // Lê um caractere por vez
        if (c == '/') {  // Pode ser o início de um comentário
            next = fgetc(input);
            if (next == '/') {  
                // Comentário de linha: pula até a quebra de linha
                while ((c = fgetc(input)) != EOF && c != '\n')
                    ;
                if (c != EOF)
                    fputc(c, output); // Escreve a quebra de linha
            } else if (next == '*') {
                // Comentário de bloco: pula até encontrar "*/"
                while ((c = fgetc(input)) != EOF) {
                    if (c == '*' && (next = fgetc(input)) == '/')
                        break;
                }
            } else {
                // Se não for comentário, escreve os dois caracteres
                fputc(c, output);
                fputc(next, output);
            }
        } else {
            // Se não for '/', escreve normalmente
            fputc(c, output);
        }
    }
}

// -------------------------------------------------------------------
// Função: processar_defines
// Objetivo: Processa uma linha que começa com "#define" para extrair a chave e o valor, armazenando-os no vetor global.
// -------------------------------------------------------------------
void processar_defines(char *line) {
    char chaves[128], valor[512];
    // Tenta ler a linha no formato "#define chave valor"
    if (sscanf(line, "#define %127s %511[^\n]", chaves, valor) == 2 && contador_defines < MAX_DEFINE) {
        strcpy(defines[contador_defines].chaves, chaves);
        strcpy(defines[contador_defines].valor, valor);
        contador_defines++; // Incrementa o contador de defines
    }
}

// -------------------------------------------------------------------
// Função: substituir_defines 
// Objetivo: Em uma linha, procura cada chave presente nos defines e substitui pelo valor correspondente.
// -------------------------------------------------------------------
void substituir_defines(char *line) {
    for (int i = 0; i < contador_defines; i++) {
        char *pos = strstr(line, defines[i].chaves); // Procura a chave na linha
        if (pos) { // Se encontrada, realiza a substituição
            char temp[MAX_LINE];
            int index = pos - line; // Posição da chave na linha
            // Copia a parte da linha antes da chave
            strncpy(temp, line, index);
            temp[index] = '\0';
            // Concatena o valor definido
            strcat(temp, defines[i].valor);
            // Concatena o restante da linha após a chave
            strcat(temp, pos + strlen(defines[i].chaves));
            strcpy(line, temp); // Atualiza a linha com a substituição
        }
    }
}

// -------------------------------------------------------------------
// Função: remove_espaco
// Objetivo: Remove espaços e tabulações extra; mantém espaço entre palavras se houver caracteres de ambos os lados.
// -------------------------------------------------------------------
void remove_espaco(char *line) {
    char compact[MAX_LINE];
    int j = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace(line[i])) {
            compact[j++] = line[i]; // Copia caracteres não-espaço
        } else {
            // Se for espaço e está entre dois caracteres alfanuméricos, mantém um espaço
            if (i > 0 && isalnum(line[i-1]) && isalnum(line[i+1])) {
                compact[j++] = ' ';
            }
        }
    }
    compact[j] = '\0';
    strcpy(line, compact);
}

// -------------------------------------------------------------------
// Declaração antecipada de processar_arquivo para ser utilizada em processar_include
// -------------------------------------------------------------------
void processar_arquivo(FILE *in, FILE *out, const char *base_path);

// -------------------------------------------------------------------
// Função: processar_include
// Objetivo: Processa uma linha que começa com "#include", extraindo o nome do arquivo incluído e chamando processar_arquivo recursivamente para incorporar o seu conteúdo.
// -------------------------------------------------------------------
void processar_include(char *line, FILE *out, const char *base_path) {
    char filename[256];
    // Extrai o nome do arquivo entre aspas: #include "arquivo.h"
    if (sscanf(line, "#include \"%255[^\"]\"", filename) == 1) {
        char path[512];
        // Forma o caminho completo usando a base passada (aqui usamos ".")
        snprintf(path, sizeof(path), "%s/%s", base_path, filename);
        FILE *inc = fopen(path, "r");
        if (!inc) {
            fprintf(stderr, "Erro: não foi possível abrir %s\n", path);
            return;
        }
        // Processa recursivamente o arquivo incluído
        processar_arquivo(inc, out, base_path);
        fclose(inc);
    }
}

// -------------------------------------------------------------------
// Função: processar_arquivo
// Objetivo: Lê o arquivo linha a linha e processa as diretivas #include e #define, aplicando substituição (substituir_defines) e remoção de espaços (remove_espaco).
// -------------------------------------------------------------------
void processar_arquivo(FILE *in, FILE *out, const char *base_path) {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), in)) {
        // Se a linha começa com "#include", processa a inclusão.
        if (strncmp(line, "#include", 8) == 0) {
         processar_include(line, out, base_path);
        }
        // Se a linha começa com "#define", armazena a definição.
        else if (strncmp(line, "#define", 7) == 0) {
            processar_defines(line);
        }
        // Caso contrário, processa a linha normal:
        else {
            substituir_defines(line);     // Substitui as chaves definidas
            remove_espaco(line); // Remove espaços desnecessários
            fprintf(out, "%s", line);  // Escreve a linha processada no arquivo de saída
        }
    }
}

// -------------------------------------------------------------------
// Função principal: main
// -------------------------------------------------------------------
int main(int argc, char *argv[]) {
    // Verifica se o usuário passou um arquivo como parâmetro.
    if (argc != 2) {
        fprintf(stderr, "Uso: %s arquivo.c\n", argv[0]);
        return 1;
    }
    char *input_path = argv[1];

    // Abre o arquivo original para leitura.
    FILE *original = fopen(input_path, "r");
    if (!original) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Cria um arquivo temporário para armazenar o código sem comentários.
    FILE *temp = tmpfile();
    if (!temp) {
        perror("Erro ao criar arquivo temporário");
        fclose(original);
        return 1;
    }

    // Etapa 1: Remover os comentários do código original.
    remover_comentarios(original, temp);
    rewind(temp); // Rebobina o arquivo temporário para o início.

    // Define o nome do arquivo de saída, acrescentando "_pre.c" ao nome original.
    char output_file[256];
    snprintf(output_file, sizeof(output_file), "%s_pre.c", strtok(input_path, "."));
    
    // Abre o arquivo de saída para escrita.
    FILE *output = fopen(output_file, "w");
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        fclose(original);
        fclose(temp);
        return 1;
    }

    // Base path para os #include (aqui usamos o diretório atual: ".")
    char base_path[256];
    strcpy(base_path, ".");

    // Etapa 2: Processa o arquivo temporário para expandir includes e defines.
    processar_arquivo(temp, output, base_path);

    // Fecha todos os arquivos abertos.
    fclose(original);
    fclose(temp);
    fclose(output);

    printf("Arquivo pré-processado salvo como: %s\n", output_file);
    return 0;
}
