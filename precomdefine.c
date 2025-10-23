#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// NOVO: Estrutura para armazenar definições
#define MAX_DEFINES 2048
#define MAX_ARGS 16

typedef struct {
    char* name;     // Nome da macro (ex: PI ou ADD)
    char* value;    // Valor/Corpo (ex: 3.14159 ou (a + b))
    int arg_count;  // Número de argumentos
    char* args[MAX_ARGS]; // Nomes dos argumentos (ex: "a", "b")
} DefineEntry;

// NOVO: Tabela global de definições
DefineEntry defines_table[MAX_DEFINES];
int define_count = 0;

// NOVO: Função auxiliar para limpar a tabela de defines
void free_defines() {
    for (int i = 0; i < define_count; i++) {
        free(defines_table[i].name);
        free(defines_table[i].value);
        for (int j = 0; j < defines_table[i].arg_count; j++) {
            free(defines_table[i].args[j]);
        }
    }
    define_count = 0;
}

// NOVO: Função auxiliar para trim (reutilizada da sua)
void trim_linha(char* str);

// NOVO: Função auxiliar para adicionar uma definição à tabela
void add_define(char* line) {
    if (define_count >= MAX_DEFINES) return;

    // Remove o "#define "
    char* p = line + strlen("#define");
    while (*p && isspace((unsigned char)*p)) p++; // Pula espaços

    char* name_start = p;
    while (*p && (isalnum((unsigned char)*p) || *p == '_')) p++;
    char* name_end = p;

    if (name_start == name_end) return; // Nenhum nome encontrado

    DefineEntry* def = &defines_table[define_count];
    def->arg_count = 0;
    
    // Verifica se é uma macro com argumentos (ex: ADD(a,b))
    if (*p == '(') {
        *name_end = '\0'; // Termina a string do nome
        def->name = strdup(name_start);

        p++; // Pula '('
        char* arg_start = p;
        while (*p && *p != ')') {
            if (*p == ',') {
                *p = '\0'; // Termina o nome do argumento
                char arg_copy[256];
                strcpy(arg_copy, arg_start);
                trim_linha(arg_copy);
                if (def->arg_count < MAX_ARGS) {
                    def->args[def->arg_count++] = strdup(arg_copy);
                }
                arg_start = p + 1;
            }
            p++;
        }
        
        if (*p == ')') {
            *p = '\0'; // Termina o último argumento
            char arg_copy[256];
            strcpy(arg_copy, arg_start);
            trim_linha(arg_copy);
            // Adiciona o último argumento se não estiver vazio
            if (strlen(arg_copy) > 0 && def->arg_count < MAX_ARGS) {
                def->args[def->arg_count++] = strdup(arg_copy);
            }
            p++; // Pula ')'
        }
    } else {
        // Define simples (constante)
        *name_end = '\0'; // Termina a string do nome
        def->name = strdup(name_start);
    }

    while (*p && isspace((unsigned char)*p)) p++; // Pula espaços antes do valor

    // O resto da linha é o valor
    char value_copy[4096];
    strcpy(value_copy, p);
    trim_linha(value_copy); // Remove espaços do início/fim do valor
    def->value = strdup(value_copy);

    define_count++;
}

// NOVO: Função auxiliar para substituir "palavra inteira" em uma string
// Retorna uma nova string alocada (precisa de free)
char* str_replace_whole_word(const char* str, const char* find, const char* replace) {
    // Buffer de saída. Pode precisar ser maior se 'replace' for maior que 'find'
    // Vamos alocar dinamicamente para segurança, mas um buffer grande funciona
    // para este exemplo.
    char buffer[8192]; 
    char* p_out = buffer;
    const char* p_in = str;
    size_t find_len = strlen(find);
    size_t replace_len = strlen(replace);

    while (*p_in) {
        // Verifica se é uma "palavra inteira"
        char char_before = (p_in == str) ? ' ' : *(p_in - 1);
        char char_after = *(p_in + find_len);

        if (strncmp(p_in, find, find_len) == 0 &&
            !isalnum((unsigned char)char_before) && char_before != '_' &&
            !isalnum((unsigned char)char_after) && char_after != '_') 
        {
            // Palavra encontrada, copia a substituição
            strcpy(p_out, replace);
            p_out += replace_len;
            p_in += find_len;
        } else {
            // Nenhum match, apenas copia o caractere
            *p_out++ = *p_in++;
        }
    }
    *p_out = '\0';
    return strdup(buffer);
}


// NOVO: Função principal de substituição de macros
// Retorna uma nova string alocada (precisa de free)
char* apply_substitutions(char* line) {
    char* current_line = strdup(line);
    int substitution_made;

    // Loop de substituição: continua substituindo até que
    // nenhuma macro possa ser expandida (para macros aninhadas)
    do {
        substitution_made = 0;
        char* next_line = strdup(current_line); // Começa com uma cópia

        for (int i = 0; i < define_count; i++) {
            DefineEntry* def = &defines_table[i];

            // Tenta substituição de constante (arg_count == 0)
            if (def->arg_count == 0) {
                char* temp = str_replace_whole_word(next_line, def->name, def->value);
                if (strcmp(next_line, temp) != 0) {
                    substitution_made = 1;
                }
                free(next_line);
                next_line = temp;
            } 
            // Tenta substituição de macro (arg_count > 0)
            else {
                // Buffer temporário para construir a linha pós-macro
                char buffer[8192];
                char* p_out = buffer;
                char* p_in = next_line;
                size_t name_len = strlen(def->name);

                while (*p_in) {
                    char char_before = (p_in == next_line) ? ' ' : *(p_in - 1);
                    
                    // Procura pela macro (ex: "ADD") seguida de "("
                    if (strncmp(p_in, def->name, name_len) == 0 &&
                        !isalnum((unsigned char)char_before) && char_before != '_' &&
                        isspace((unsigned char)*(p_in + name_len)) == 0 && // Permite ADD(..), não ADD (..)
                        *(p_in + name_len) == '(') 
                    {
                        // --- Macro encontrada ---
                        p_in += name_len; // Pula o nome
                        while (*p_in && isspace((unsigned char)*p_in)) p_in++; // Pula espaço
                        p_in++; // Pula '('

                        char passed_args[MAX_ARGS][256];
                        int arg_idx = 0;
                        char* arg_start = p_in;
                        int paren_level = 0;

                        // Analisa os argumentos passados
                        while (*p_in) {
                            if (*p_in == '(') paren_level++;
                            else if (*p_in == ')') {
                                if (paren_level == 0) break;
                                else paren_level--;
                            } else if (*p_in == ',' && paren_level == 0) {
                                // Argumento encontrado
                                if (arg_idx < MAX_ARGS) {
                                    strncpy(passed_args[arg_idx], arg_start, p_in - arg_start);
                                    passed_args[arg_idx][p_in - arg_start] = '\0';
                                    trim_linha(passed_args[arg_idx]);
                                    arg_idx++;
                                }
                                arg_start = p_in + 1;
                            }
                            p_in++;
                        }
                        
                        if (*p_in == ')') {
                            // Pega o último argumento
                            if (arg_idx < MAX_ARGS) {
                                strncpy(passed_args[arg_idx], arg_start, p_in - arg_start);
                                passed_args[arg_idx][p_in - arg_start] = '\0';
                                trim_linha(passed_args[arg_idx]);
                                arg_idx++;
                            }
                            p_in++; // Pula ')'

                            // Se o número de argumentos bate, faz a substituição
                            if (arg_idx == def->arg_count) {
                                substitution_made = 1;
                                char* substituted_value = strdup(def->value);
                                
                                // Substitui os nomes dos args (ex: "a") pelos valores passados (ex: "5")
                                for (int j = 0; j < def->arg_count; j++) {
                                    char* temp = str_replace_whole_word(substituted_value, def->args[j], passed_args[j]);
                                    free(substituted_value);
                                    substituted_value = temp;
                                }
                                
                                strcpy(p_out, substituted_value);
                                p_out += strlen(substituted_value);
                                free(substituted_value);
                            } else {
                                // Número errado de args, não substitui. Copia o nome original.
                                strncpy(p_out, def->name, name_len);
                                p_out += name_len;
                                *p_out++ = '('; // Copia o '(' que pulamos
                            }
                        } else {
                            // ')' não encontrado, malformado. Copia o nome original.
                            strncpy(p_out, def->name, name_len);
                            p_out += name_len;
                        }
                    } else {
                        *p_out++ = *p_in++;
                    }
                } // fim while(*p_in)
                *p_out = '\0';
                free(next_line);
                next_line = strdup(buffer);
            }
        } // fim for(defines)
        
        free(current_line);
        current_line = next_line;

    } while (substitution_made);

    return current_line;
}


// NOVO: Função de pré-processamento para #define
void expdefine(FILE *input, FILE *output) {
    char line[4096];
    
    // 1. Primeiro Passo: Coletar todas as definições
    while (fgets(line, sizeof(line), input) != NULL) {
        char* start = line;
        while (*start == ' ' || *start == '\t') start++;

        if (strncmp(start, "#define", 7) == 0) {
            add_define(start);
        }
    }

    // 2. Segundo Passo: Substituir
    fseek(input, 0, SEEK_SET); // Volta ao início do arquivo
    
    while (fgets(line, sizeof(line), input) != NULL) {
        char* start = line;
        while (*start == ' ' || *start == '\t') start++;

        // Pula as linhas #define
        if (strncmp(start, "#define", 7) == 0) {
            continue;
        }

        // Mantém outras diretivas (ex: #pragma)
        if (*start == '#') {
            fputs(line, output);
            continue;
        }

        // Aplica substituições na linha
        char* processed_line = apply_substitutions(line);
        fputs(processed_line, output);
        free(processed_line);
    }
}


// -------------------------------------------------------------------
// SUAS FUNÇÕES ORIGINAIS (com trim_linha movida para cima)
// -------------------------------------------------------------------

void expinclude(FILE *input, FILE *output) {
    char line[4096];

         while (fgets(line, sizeof(line), input) != NULL) {
              char* start = line;
              while (*start == ' ' || *start == '\t') start++;

              if (strncmp(start, "#include", 8) == 0) {
                       char* p = start + 8;
                       char filename[256];
                       while (*p == ' ' || *p == '\t') p++;

                       if (*p == '"') {
                            char* fn_start = p + 1;
                            char* fn_end = strchr(fn_start, '"'); 
                            
                            if (fn_end != NULL) {
                                     size_t len = fn_end - fn_start;
                                     strncpy(filename, fn_start, len);
                                     filename[len] = '\0';

                                     FILE* included_file = fopen(filename, "r");
                                     if (included_file != NULL) {
                                          expinclude(included_file, output); 
                                          fclose(included_file);
                                     } else {
                                          fprintf(stderr, "Aviso: Nao foi possivel encontrar o arquivo include: %s\n", filename);
                                          fputs(line, output);
                                     }
                            } else {
                                     fputs(line, output);
                            }
                       } else {
                            fputs(line, output);
                       }
              } else {
                       fputs(line, output);
              }
         }
}

void removecomentario(FILE *input, FILE *output) {
         char line[4096];

         while (fgets(line, sizeof(line), input) != NULL) {

              char* pos_comentario = strstr(line, "//");

              if (pos_comentario != NULL) {

                       int aspas = 0;
                       for (char* p = line; p < pos_comentario; p++) {
                            if (*p == '"') {
                                     aspas = !aspas;
                            }
                       }
                       
                       if (!aspas) {
                            *pos_comentario = '\n';
                            *(pos_comentario + 1) = '\0';
                       }
              }

              fputs(line, output);
         }
}

// MOVIDA: Esta função agora está antes de add_define()
void trim_linha(char* str) {
         char* start = str;
         // Remove espaços do início
         while (isspace((unsigned char)*start)) start++;
    
         // Remove espaços do fim
         char* end = start + strlen(start) - 1;
         while (end > start && isspace((unsigned char)*end)) end--;
    
         // Escreve o terminador nulo
         *(end + 1) = '\0';
    
         // Move a string para o início (se houver espaços no início)
         if (str != start) memmove(str, start, strlen(start) + 1);
}

void minificar_linha_c(char* str) {
         char buffer[4096];
         char* p_leitura = str;
         char* p_escrita = buffer;
         int dentro_de_aspas = 0;

         while (*p_leitura) {
              if (*p_leitura == '"') dentro_de_aspas = !dentro_de_aspas;
              
              if (!dentro_de_aspas) {

                       if (isspace((unsigned char)*p_leitura) && 
                           (*(p_leitura + 1) == '{' || *(p_leitura + 1) == '(' || *(p_leitura + 1) == ';')) {
                            p_leitura++;
                            continue;
                       }

                       if ((*p_leitura == ')' || *p_leitura == '}' || *p_leitura == ';') && 
                                isspace((unsigned char)*(p_leitura + 1))) {
                            *p_escrita++ = *p_leitura++;
                            while (isspace((unsigned char)*p_leitura)) p_leitura++;
                            continue;
                       }
              }
              *p_escrita++ = *p_leitura++;
         }
         *p_escrita = '\0';
         strcpy(str, buffer);
}

void removeespacos(FILE *input, FILE *output) {
         char line[4096];
    
         while (fgets(line, sizeof(line), input) != NULL) {
              char linha_trabalho[4096];
              strcpy(linha_trabalho, line);
              
              trim_linha(linha_trabalho);

              if (strlen(linha_trabalho) == 0) {
                       continue;
              }

              // MODIFICADO: A função expdefine já tratou os defines,
        // então só precisamos preservar outras diretivas (ex: #pragma)
              if (linha_trabalho[0] == '#') {
                       fputs(line, output);
              } else {
                       minificar_linha_c(linha_trabalho);
                       fputs(linha_trabalho, output);
              }
         }
}

// MODIFICADO: main() atualizada para incluir o novo passo
int main(int argc, char *argv[]) {
         if (argc < 3) {
              fprintf(stderr, "Uso: %s <arquivo_entrada.txt> <arquivo_saida.txt>\n", argv[0]);
              return 1;
         }

         char* input_filename = argv[1];
         char* output_filename = argv[2];

    // MODIFICADO: Adicionado temp3
         char temp1_name[] = "temp_passo1.tmp";
         char temp2_name[] = "temp_passo2.tmp";
    char temp3_name[] = "temp_passo3.tmp"; // NOVO

         FILE *f_in, *f_out;
         FILE *f_temp1_in, *f_temp1_out;
         FILE *f_temp2_in, *f_temp2_out;
    FILE *f_temp3_in, *f_temp3_out; // NOVO

         printf("Passo 1: Expandindo includes (#include)...\n");
         f_in = fopen(input_filename, "r");
         if (f_in == NULL) {
              perror("Erro ao abrir arquivo de entrada");
              return 1;
         }
         f_temp1_out = fopen(temp1_name, "w");
         if (f_temp1_out == NULL) {
              perror("Erro ao criar arquivo temporario 1");
              fclose(f_in);
              return 1;
         }
         expinclude(f_in, f_temp1_out);
         fclose(f_in);
         fclose(f_temp1_out);

    // NOVO: Passo 2 para #define
    printf("Passo 2: Expandindo defines (#define)...\n");
    f_temp1_in = fopen(temp1_name, "r");
    f_temp2_out = fopen(temp2_name, "w");
    if (f_temp1_in == NULL || f_temp2_out == NULL) {
        perror("Erro ao abrir arquivos temporarios (Passo 2)");
        return 1;
    }
    expdefine(f_temp1_in, f_temp2_out);
    free_defines(); // Limpa a memória alocada para a tabela
    fclose(f_temp1_in);
    fclose(f_temp2_out);


    // MODIFICADO: Passo 3 (era 2)
         printf("Passo 3: Removendo comentarios (//)...\n");
         f_temp2_in = fopen(temp2_name, "r");
         f_temp3_out = fopen(temp3_name, "w");
         if (f_temp2_in == NULL || f_temp3_out == NULL) {
              perror("Erro ao abrir arquivos temporarios (Passo 3)");
              return 1;
         }
         removecomentario(f_temp2_in, f_temp3_out);
         fclose(f_temp2_in);
         fclose(f_temp3_out);

    // MODIFICADO: Passo 4 (era 3)
         printf("Passo 4: Removendo espacos e minificando...\n");
         f_temp3_in = fopen(temp3_name, "r"); 
         f_out = fopen(output_filename, "w"); 
         if (f_temp3_in == NULL || f_out == NULL) {
              perror("Erro ao abrir arquivos (Passo 4)");
              return 1;
         }
         removeespacos(f_temp3_in, f_out);
         fclose(f_temp3_in);
         fclose(f_out);

         printf("Limpando arquivos temporarios...\n");
         remove(temp1_name);
         remove(temp2_name);
    remove(temp3_name); // NOVO

         printf("Processamento concluido! Saida salva em: %s\n", output_filename);

         return 0;
}