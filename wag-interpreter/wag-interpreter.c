#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct{
    char nome[100];
    int valor;
    struct lista_var *prox;
} var_int;

var_int* insere(var_int* l){
    if(l == NULL){
        p = (var_int*) malloc(sizeof(var_int))
    }
}

int ler_var_int(FILE* code){
    char temp;
    fscanf(code, "%c", &temp);
    int valor;
    fscanf(code, "%d", &valor);
    return valor;
}

int main(int argc, char** argv){
    if(argc == 1){
        printf("No input file was given.\n");
        exit(-1);
    }
    FILE* code = fopen(argv[1], "r");
    char leitura[100];
    fscanf(code, "%s", leitura);
    
    if(strcmp(leitura, "var")){
        char nome_da_var[100];
        fscanf(code, "%s", nome_da_var);
        int (nome_da_var) = ler_var_int(code);
        printf("%d", (nome_da_var));
    }

}