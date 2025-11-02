#include "contagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BlocoMemoria {
    void *endereco_real;
    size_t tamanho;
    int contagem_referencias;
    struct BlocoMemoria *proximo;
} BlocoMemoria;

static BlocoMemoria *lista_blocos_gerenciados = NULL;

static BlocoMemoria *encontrar_bloco(void *endereco) {
    BlocoMemoria *atual = lista_blocos_gerenciados;
    while (atual != NULL) {
        if (atual->endereco_real == endereco) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

static void gc_executa_coleta() {
    BlocoMemoria *anterior = NULL;
    BlocoMemoria *atual = lista_blocos_gerenciados;

    while (atual != NULL) {
        if (atual->contagem_referencias <= 0) {
            
            free(atual->endereco_real);

            if (anterior == NULL) {
                lista_blocos_gerenciados = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            BlocoMemoria *bloco_a_liberar = atual;
            atual = atual->proximo; 
            free(bloco_a_liberar);
        } else {
            anterior = atual;
            atual = atual->proximo;
        }
    }
}

void *gc_aloca(size_t tamanho) {
    void *novo_endereco = malloc(tamanho);
    if (novo_endereco == NULL) {
        return NULL;
    }

    BlocoMemoria *novo_bloco = (BlocoMemoria *)malloc(sizeof(BlocoMemoria));
    if (novo_bloco == NULL) {
        free(novo_endereco);
        return NULL;
    }

    novo_bloco->endereco_real = novo_endereco;
    novo_bloco->tamanho = tamanho;
    novo_bloco->contagem_referencias = 1;
    
    novo_bloco->proximo = lista_blocos_gerenciados;
    lista_blocos_gerenciados = novo_bloco;

    return novo_endereco;
}

void gc_atribui(void **ponteiro_destino, void *ponteiro_origem) {
    void *endereco_antigo = *ponteiro_destino;

    if (endereco_antigo != NULL) {
        BlocoMemoria *bloco_antigo = encontrar_bloco(endereco_antigo);
        if (bloco_antigo != NULL) {
            bloco_antigo->contagem_referencias--;
        }
    }
    
    *ponteiro_destino = ponteiro_origem;

    if (ponteiro_origem != NULL) {
        BlocoMemoria *bloco_novo = encontrar_bloco(ponteiro_origem);
        if (bloco_novo != NULL) {
            bloco_novo->contagem_referencias++;
        }
    }
    
    gc_executa_coleta();
}

void gc_status_memoria() {
    printf("\nSITUACAO DA MEMORIA\n");
    if (lista_blocos_gerenciados == NULL) {
        printf("Nenhum bloco de memoria esta sendo encontrado.\n");
        printf("------------------------------------\n");
        return;
    }

    BlocoMemoria *atual = lista_blocos_gerenciados;
    int i = 0;
    while (atual != NULL) {
        printf("Bloco %d: Endereco: %p, Tamanho: %zu, Contagem de Ref.: %d\n", 
               i++, atual->endereco_real, atual->tamanho, atual->contagem_referencias);
        atual = atual->proximo;
    }
    printf("------------------------------------\n");
}

void gc_limpeza_final() {
    while (lista_blocos_gerenciados != NULL) {
        gc_executa_coleta(); 
        
        if (lista_blocos_gerenciados != NULL && lista_blocos_gerenciados->contagem_referencias > 0) {
            BlocoMemoria *atual = lista_blocos_gerenciados;
            while(atual != NULL) {
                 atual->contagem_referencias = 0;
                 atual = atual->proximo;
            }
             gc_executa_coleta();
        }
    }
}