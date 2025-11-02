// Desenvolvido em colaboração por :

/*
grupo de :
Enzo Cariello &&
João Paulo firmiano
*/ 

/*
grupo de :
Leonardo Brandão &&
Samuel Galasso
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DEFINES 2048
#define MAX_ARGS 16

typedef struct {
    char* name;     // Nome da macro 
    char* value;    // Valor direto, ou exprecao do macro
    int arg_count;  // Numero de argumentos
    char* args[MAX_ARGS]; // Nomes dos argumentos 
} DefineEntry;

DefineEntry defines_table[MAX_DEFINES];
int define_count = 0;

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

void trim_linha(char* str) {
    char* start = str;

    while (isspace((unsigned char)*start)) start++;

    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    if (str != start) memmove(str, start, strlen(start) + 1);
}

void add_define(char* line) {
    if (define_count >= MAX_DEFINES) return;

    char* p = line + 7;
    while (*p && isspace((unsigned char)*p)) p++; // linha responsavel por pular os espaços

    char* name_start = p;
    while (*p && (isalnum((unsigned char)*p) || *p == '_')) p++;
    char* name_end = p;

    if (name_start == name_end) return; // Caso nenhum nome seja encontrado

    DefineEntry* def = &defines_table[define_count];
    def->arg_count = 0;
    
    if (*p == '(') {
        *name_end = '\0'; // identifica o final da string do nome
        def->name = strdup(name_start);

        p++; // Pula '('
        
        char* arg_start = p;
        while (*p && *p != ')') {
            if (*p == ',') {
                *p = '\0'; // identifica o final do nome do argumento
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
            *p = '\0'; // identifica o final do último argumento
            char arg_copy[256];
            strcpy(arg_copy, arg_start);
            trim_linha(arg_copy);
            // Adiciona o último argumento caso ele nao esteja vazio
            if (strlen(arg_copy) > 0 && def->arg_count < MAX_ARGS) {
                def->args[def->arg_count++] = strdup(arg_copy);
            }
            p++; // Pula os ')'
        }
    } else {
        // Define constante
        *name_end = '\0'; // identifica o final da string do nome
        def->name = strdup(name_start);
    }

    while (*p && isspace((unsigned char)*p)) p++; // Pula os espaços antes do valor

    char value_copy[4096];
    strcpy(value_copy, p);
    trim_linha(value_copy); // Remove os espaços do início/fim do valor
    def->value = strdup(value_copy);

    define_count++;
}

char* str_replace_whole_word(const char* str, const char* find, const char* replace) {

    char buffer[8192]; 
    char* p_out = buffer;
    const char* p_in = str;
    size_t find_len = strlen(find);
    size_t replace_len = strlen(replace);

    while (*p_in) {
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

char* apply_substitutions(char* line) {
    char* current_line = strdup(line);
    int substitution_made;

    do {
        substitution_made = 0;
        char* next_line = strdup(current_line);

        for (int i = 0; i < define_count; i++) {
            DefineEntry* def = &defines_table[i];

            if (def->arg_count == 0) {
                char* temp = str_replace_whole_word(next_line, def->name, def->value);
                if (strcmp(next_line, temp) != 0) {
                    substitution_made = 1;
                }
                free(next_line);
                next_line = temp;
            } 

            else {

                char buffer[8192];
                char* p_out = buffer;
                char* p_in = next_line;
                size_t name_len = strlen(def->name);

                while (*p_in) {
                    char char_before = (p_in == next_line) ? ' ' : *(p_in - 1);
                    
                    if (strncmp(p_in, def->name, name_len) == 0 && !isalnum((unsigned char)char_before) && char_before != '_' && isspace((unsigned char)*(p_in + name_len)) == 0 && *(p_in + name_len) == '(') 
                    {
                        p_in += name_len; // Pula o nome
                        while (*p_in && isspace((unsigned char)*p_in)) p_in++; // Pula espaço
                        p_in++; // Pula '('

                        char passed_args[MAX_ARGS][256];
                        int arg_idx = 0;
                        char* arg_start = p_in;
                        int paren_level = 0;

                        while (*p_in) {
                            if (*p_in == '(') paren_level++;
                            else if (*p_in == ')') {
                                if (paren_level == 0) break;
                                else paren_level--;
                            } else if (*p_in == ',' && paren_level == 0) {

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

                            if (arg_idx < MAX_ARGS) {
                                strncpy(passed_args[arg_idx], arg_start, p_in - arg_start);
                                passed_args[arg_idx][p_in - arg_start] = '\0';
                                trim_linha(passed_args[arg_idx]);
                                arg_idx++;
                            }
                            p_in++;

                            if (arg_idx == def->arg_count) {
                                substitution_made = 1;
                                char* substituted_value = strdup(def->value);

                                for (int j = 0; j < def->arg_count; j++) {
                                    char* temp = str_replace_whole_word(substituted_value, def->args[j], passed_args[j]);
                                    free(substituted_value);
                                    substituted_value = temp;
                                }
                                
                                strcpy(p_out, substituted_value);
                                p_out += strlen(substituted_value);
                                free(substituted_value);
                            } else {

                                strncpy(p_out, def->name, name_len);
                                p_out += name_len;
                                *p_out++ = '('; // Copia o '(' que pulamos
                            }
                        } else {

                            strncpy(p_out, def->name, name_len);
                            p_out += name_len;
                        }
                    } else {
                        *p_out++ = *p_in++;
                    }
                }
                *p_out = '\0';
                free(next_line);
                next_line = strdup(buffer);
            }
        }
        
        free(current_line);
        current_line = next_line;

    } while (substitution_made);

    return current_line;
}

void expdefine(FILE *input, FILE *output) {
    char line[4096];

    while (fgets(line, sizeof(line), input) != NULL) {
        char* start = line;
        while (*start == ' ' || *start == '\t') start++;

        if (strncmp(start, "#define", 7) == 0) {
            add_define(start);
        }
    }

    fseek(input, 0, SEEK_SET);
    
    while (fgets(line, sizeof(line), input) != NULL) {
        char* start = line;
        while (*start == ' ' || *start == '\t') start++;

        if (strncmp(start, "#define", 7) == 0) {
            continue;
        }

        if (*start == '#') {
            fputs(line, output);
            continue;
        }

        char* processed_line = apply_substitutions(line);
        fputs(processed_line, output);
        free(processed_line);
    }
}

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

void minificar_linha_c(char* str) {
    char buffer[4096];
    char* p_leitura = str;
    char* p_escrita = buffer;
    int dentro_de_aspas = 0;

    while (*p_leitura) {
        if (*p_leitura == '"') dentro_de_aspas = !dentro_de_aspas;
              
        if (!dentro_de_aspas) {
            if (isspace((unsigned char)*p_leitura) && (*(p_leitura + 1) == '{' || *(p_leitura + 1) == '(' || *(p_leitura + 1) == ';')) {
                p_leitura++;
                continue;
            }

            if ((*p_leitura == ')' || *p_leitura == '}' || *p_leitura == ';') && isspace((unsigned char)*(p_leitura + 1))) {
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

    printf("Passo 2: Expandindo defines (#define)...\n");
    f_temp1_in = fopen(temp1_name, "r");
    f_temp2_out = fopen(temp2_name, "w");

    if (f_temp1_in == NULL || f_temp2_out == NULL) {
        perror("Erro ao abrir arquivos temporarios (Passo 2)");
        return 1;
    }

    expdefine(f_temp1_in, f_temp2_out);
    free_defines();
    fclose(f_temp1_in);
    fclose(f_temp2_out);

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