#ifndef _CONTREF_H_
#define _CONTREF_H_


#include <stdio.h>
#include <stdlib.h>


typedef struct lista{

    int status;
    void* endereco;
    struct lista* prox;

}Lista;

Lista* heap = NULL;

void insert(Lista** heap, Lista* L){

    Lista* aux = malloc(sizeof(Lista));

    if(*heap == NULL){
        *heap = L;
    }
    else{
        aux = *heap;
        while(aux->prox){
            aux = aux->prox;
        }
        aux->prox = L;
    }
    return;

}


Lista* coleta(Lista* L){

    Lista* aux, *remover;

    if(L){
        if(L->status == 0){
            remover = coleta(L->prox);
            free(L);
            return remover;
        }
        else{
            aux = L;
            aux->prox = coleta(aux->prox);
            return aux;
        }
    }

    return NULL;
}


void* malloc2(int tam){


    Lista* L = (Lista*)malloc(sizeof(Lista));
    L->status = 1;
    L->prox = NULL;
    L->endereco = malloc(tam);

    insert(&heap, L);

    return L->endereco;


}

void arranjo(void* e1, void* e2){

    Lista* aux = heap;

    while(aux){
        if(aux->endereco == e1){
            aux->status = aux->status - 1;
        }
        if(e2){
            if(aux->endereco == e2){
                aux->status = aux->status + 1;
            }
        }
        aux = aux->prox;
    }

}

void atrib2(void* e1, void* e2){

    arranjo(e1, e2);
    e1 = e2;
    heap = coleta(heap);
    return;

}

void dump(){

    Lista* aux = heap;
    while(aux){
        printf("Endereco: %p, Contagem de Referencias: %d.\n", aux->endereco, aux->status);
        aux = aux->prox;
    }
    printf("\n");

}


#endif // _CONTREF_H_

