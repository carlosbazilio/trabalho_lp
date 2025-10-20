#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removecomentario (FILE *input, FILE *output){
    int caracter, seguinte;

    while((caracter = fgetc(input)) != EOF){
        if (caracter == '/'){
            seguinte = fgetc(input);
            if (seguinte == '/'){
                while ((caracter = fgetc(input)) != EOF && caracter != '\n');
                fputc(' ', output);
            }else if (seguinte == '*'){
                int prev = 0;
                while ((caracter = fgetc(input)) != EOF) {
                    if (prev == '*' && caracter == '/') {
                        break;
                    }
                    prev = caracter;
                }
                fputc(' ', output);
            }else {
                fputc(caracter, output);
                if (seguinte != EOF) {
                    fputc(seguinte, output);
                }
            }
        }else {
            fputc(caracter, output);
        }
    }
}

void expinclude(FILE *input, FILE *output) {
    char line[4096];

    while (fgets(line, sizeof(line), input) != NULL) {
        // Remove espaços em branco do início da linha
        char* start = line;
        while (*start == ' ' || *start == '\t') {
            start++;
        }

        if (strncmp(start, "#include", 8) == 0) {
            char* p = start + 8;
            char filename[256];

            while (*p == ' ' || *p == '\t') p++;

            if (*p == '"') {
                char* fn_start = p + 1;

                char* fn_end = strchr(fn_start, '"'); 
                
                if (fn_end != NULL) {
                    // Extrai o nome do arquivo
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