#include <stdlib.h>
#include <stdio.h>
#include "contref.h"

contador *lista = NULL;

contador *removeE(contador *a, void *endereco){
    contador *aux = a;
    
    while(aux != NULL && aux->endereco != endereco)
        aux = aux->prox;

    if(aux != NULL){
        if(aux->ant == NULL && aux->prox == NULL){
            free(aux->endereco);
            free(aux);
            return NULL;
        } else if(aux->ant != NULL && aux->prox == NULL){
            aux->ant->prox = NULL;
            free(aux->endereco);
            free(aux);
        } else if(aux->ant == NULL && aux->prox != NULL){
            aux->prox->ant = NULL;
            a = aux->prox;
            free(aux->endereco);
            free(aux);
        } else{
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;
            free(aux->endereco);
            free(aux);
        }
    }
    return a;
}

void incrementa(void *b1, int n){
    contador *aux = lista;

    while(aux != NULL && aux->endereco != b1)
        aux = aux->prox;

    if(aux != NULL && aux->endereco == b1){
        aux->cont = aux->cont + n;
        if(aux->cont == 0)
            erasenode(lista, b1);
    }
}

void *malloc2(int size){
    contador *aux = malloc(sizeof(contador));
    aux->endereco = malloc(size);
    aux->cont = 1;

    if(lista != NULL){
        aux->prox = lista;
        aux->ant = NULL;
        lista->ant = aux;
    } else{
        aux->prox = NULL;
        aux->ant = NULL;
    }

    lista = aux;

    return lista->endereco;
}

void atrib2(void *b1, void *b2){
    if(b1 != NULL){
        update(b1, -1);
        if(b2 != NULL)
            update(b2, 1);
        b1 = b2;
    }
}

void dump(){
    contador *aux = lista;

    while(aux != NULL){
        printf("endereco: %p\ncont: %i\n\n", aux->endereco, aux->cont);
        aux = aux->prox;
    }
}