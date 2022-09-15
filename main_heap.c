#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gerenciador{
    struct gerenciador *prox;
    int endereco;
    int espacos;
}gerenciador;


void heap(char *modo){// 
    if(strcmp(modo,"first")==0){
        return 1;
    }
    if(strcmp(modo,"best")==0){
        return 2;
    }
    if(strcmp(modo,"worst")==0){
        return 3;
    }
    if(strcmp(modo,"fit")==0){
        return 4;
    }



}
int new(char id, int tamanho)//vamos criar primeiro pra depois deletar
{
    vamos :)

    return 0;
}

int delete()//retorna 1 se foi deletado com sucesso
{

}
int trataEntrada(char[] comando){
    char *quebrado;
    int i=0;
    char parametros [] [];
    quebrado = strtok(comando,' ');
    while (quebrado != NULL){
        parametros[i] = quebrado;
        i++;    //cada palavra vai ficar aki
        
    }
    for( int j = 0; j<i; j++){//i numero de palavras
        if(strcomp(parametros[j],"new")==0){
            int a,b;
            a =parametros[j+1];
            b = atoi(parametros[j+2]);
            new(a,b);//new a 5

        }
        if(strcomp(parametros[j],"heap")==0){
            heap(parametros[j+1]);//heap foi
        }
        if(strcomp(parametros[j],"lista")==0){
            lista();//lista foi
        }
        if(strcomp(parametros[j],"delete")==0){
            delete(parametros[j+1]);//delete
        }
        if(strcomp(parametros[j],"atribui")==0){
            atribui(parametros[j+1],parametros[j+2]);//atribui a b
        }
    }
}