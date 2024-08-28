#include "coletalixo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

//Estrutura de blocos
typedef struct endereco{
    void *ptr;
    size_t tam;
    int cont;
    struct endereco *prox;
}Endereco;

static Endereco* inicio = NULL;

Endereco* encontrarEnd(void *ptr){
    Endereco* atual = inicio;

    while(atual != NULL){
        if(atual->ptr == ptr){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void deletarEndereco(Endereco *endereco){
    printf("Limpando...\n\n");
    free(endereco);
}

void* malloc2(size_t tam){
    Endereco* novoEndereco = (Endereco*) malloc(sizeof(Endereco));
    novoEndereco->ptr = malloc(tam);
    novoEndereco->tam = tam;
    novoEndereco->cont = 1;
    novoEndereco->prox = inicio;
    inicio = novoEndereco;
    return novoEndereco->ptr;
}

void atrib(void** ptr1, void* ptr2){
    if(ptr1 == NULL){
        return;
    }

    Endereco *endereco1 = encontrarEnd(*ptr1);
    if(endereco1){
        endereco1->cont--;
        if(endereco1->cont <= 0){
            deletarEndereco(endereco1);
        }
    }

    if(ptr2 == NULL){
        *ptr1 = NULL;
        return;
    }

    Endereco *endereco2 = encontrarEnd(ptr2);
    if(endereco2){
        endereco2->cont++;
    }
    *ptr1 = ptr2;

}

void dump(){
    Endereco *atual = inicio;
    while(atual != NULL){
        printf("Endereco: %p, Tamanho: %zu, Contagem de Referencias: %d\n", atual->ptr, atual->tam, atual->cont);
        atual = atual->prox;
    }
    printf("\n");
}