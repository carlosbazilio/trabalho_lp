#include <stdio.h>
#include <stdlib.h>
#include "coleta.h"

typedef struct Dados{
    int cont;
    int size;
    struct Dados* prox;
}Dados;

static Dados* Memoria = NULL;

void* malloc2(int tamanho){
    Dados* a = (Dados*)malloc(sizeof(Dados) + tamanho);

    if(a == NULL)
        return NULL;

    a->cont = 1;
    a->size = tamanho;
    a->prox = Memoria;
    Memoria = a;

    return (void*)((char*)a + sizeof(Dados));
}

static Dados* bloco(void* a){
    if(a==NULL)
        return NULL;
    else
        return (Dados*)((char*)a - sizeof(Dados));
}

static void RemoverNo(Dados* c) {
    if(Memoria == NULL || c ==NULL)
        return;
    if (Memoria == c) {
        Memoria = c->prox;
        free(c);
    }
    else {
        Dados* pos = Memoria;
        Dados* prev = NULL;
        while (pos != NULL && pos != c) {
            prev = pos;
            pos = pos->prox;
        }
        if (pos == c) {
            prev->prox = c->prox;
            free(c);
        }
    }
}


void _atrib_real(void** a, void* b) {
    Dados* d = bloco(b);
    Dados* c = bloco(*a);

    if (c != NULL && d != NULL && c->size != d->size) {
        printf("\nERRO: Atribuição entre tipos de tamanhos incompatíveis!\n");
        return;
    }

    if (d != NULL) {
        d->cont++;
    }

    if (c != NULL) {
        c->cont--;

        if (c->cont == 0) {
            printf("\n>>> Memória liberada (Bloco %p)!\n", (void*)c);
            RemoverNo(c);
        }
    }

    *a = b;
}


void dump() {
    printf("--- VERIFICACAO DA MEMORIA ---\n");

    Dados* temp = Memoria;

    if (temp == NULL) {
        printf("  (Nenhum bloco de memoria alocado)\n");
    }
    while (temp != NULL) {
        void* ptr_usuario = (void*)((char*)temp + sizeof(Dados));

        printf("  Bloco [ %p ] (Dados em [ %p ]) | Tamanho: %d bytes | Contador: %d\n",
               (void*)temp,
               ptr_usuario,
               temp->size,
               temp->cont);


        temp = temp->prox;
    }

    printf("--- FIM DA VERIFICACAO ---\n\n");
}
