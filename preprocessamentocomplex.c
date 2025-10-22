#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void trim_linha(char* str) {
    char* start = str;
    while (isspace((unsigned char)*start)) start++;
    
    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = '\0';
    
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

        if (linha_trabalho[0] == '#') {

            fputs(line, output);
        } else {

            minificar_linha_c(linha_trabalho);

            fputs(linha_trabalho, output);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.txt> <arquivo_saida.txt>\n", argv[0]);
        return 1;
    }

    char* input_filename = argv[1];
    char* output_filename = argv[2];

    char temp1_name[] = "temp_passo1.tmp";
    char temp2_name[] = "temp_passo2.tmp";

    FILE *f_in, *f_out;
    FILE *f_temp1_in, *f_temp1_out;
    FILE *f_temp2_in, *f_temp2_out;

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

    printf("Passo 2: Removendo comentarios (//)...\n");
    f_temp1_in = fopen(temp1_name, "r");
    f_temp2_out = fopen(temp2_name, "w");
    if (f_temp1_in == NULL || f_temp2_out == NULL) {
        perror("Erro ao abrir arquivos temporarios (Passo 2)");
        return 1;
    }
    removecomentario(f_temp1_in, f_temp2_out);
    fclose(f_temp1_in);
    fclose(f_temp2_out);

    printf("Passo 3: Removendo espacos e minificando...\n");
    f_temp2_in = fopen(temp2_name, "r"); 
    f_out = fopen(output_filename, "w"); 
    if (f_temp2_in == NULL || f_out == NULL) {
        perror("Erro ao abrir arquivos (Passo 3)");
        return 1;
    }
    removeespacos(f_temp2_in, f_out);
    fclose(f_temp2_in);
    fclose(f_out);

    printf("Limpando arquivos temporarios...\n");
    remove(temp1_name);
    remove(temp2_name);

    printf("Processamento concluido! Saida salva em: %s\n", output_filename);

    return 0;
}