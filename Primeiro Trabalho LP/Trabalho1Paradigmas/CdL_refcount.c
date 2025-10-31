#include "CdL_refcount.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct No {
    void* ponteiro;
    int cont_referencias;
    struct No* proximo;
} No;

static No* inicio = NULL;

static No* buscar_no(void* ponteiro) {
    No* atual = inicio;
    while (atual) {
        if (atual->ponteiro == ponteiro) return atual;
        atual = atual->proximo;
    }
    return NULL;
}

void* malloc2(size_t tamanho) {
    void* ponteiro = malloc(tamanho);
    if (!ponteiro) return NULL;

    No* novo = malloc(sizeof(No));
    novo->ponteiro = ponteiro;
    novo->cont_referencias = 1;
    novo->proximo = inicio;
    inicio = novo;

    return ponteiro;
}

void atrib2(void** destino, void* origem) {
    if (destino == NULL) return;

    if (*destino) {
        No* no_antigo = buscar_no(*destino);
        if (no_antigo) {
            no_antigo->cont_referencias--;
            if (no_antigo->cont_referencias == 0) {
                free(no_antigo->ponteiro);
                // Remove da lista
                if (inicio == no_antigo) {
                    inicio = no_antigo->proximo;
                } else {
                    No* temp = inicio;
                    while (temp && temp->proximo != no_antigo)
                        temp = temp->proximo;
                    if (temp)
                        temp->proximo = no_antigo->proximo;
                }
                free(no_antigo);
            }
        }
    }

    *destino = origem;

    if (origem) {
        No* no_novo = buscar_no(origem);
        if (no_novo) {
            no_novo->cont_referencias++;
        }
    }
}

void CdL_status() {
    No* atual = inicio;
    printf("Status do Coletor de Lixo (CdL):\n");
    while (atual) {
        printf(" - %p [referências: %d]\n", atual->ponteiro, atual->cont_referencias);
        atual = atual->proximo;
    }
}

void CdL_limpar() {
    No* atual = inicio;
    while (atual) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp->ponteiro);
        free(temp);
    }
    inicio = NULL;
}
