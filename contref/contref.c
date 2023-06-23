#include <stdio.h>
#include <stdlib.h>
#include "contref.h"

Memoria *listaReferencia = NULL;

void imprimirListaReferencia(){

    Memoria *aux = listaReferencia;

    while (aux != NULL){

        printf("end: %p\tref: %d\n", aux->endereco, aux->contadorRef);
        aux = aux->prox;

    }
}

void atualizarListaReferencia(Memoria *elemento, int contador){
    if (elemento != NULL){

        elemento->contadorRef += contador;

        if (elemento->contadorRef == 0){

            if (listaReferencia == elemento){

                listaReferencia = elemento->prox;

            }else{

                Memoria *corredor = listaReferencia;

                while(corredor->prox != elemento){

                    corredor = corredor->prox;

                }

                corredor->prox = elemento->prox;

            }

            free(elemento->endereco);
            free(elemento);

        }
    }
}

void *malloc2(int tamanho){

    void *novaMemoriaAlocada = malloc(tamanho);

    if(novaMemoriaAlocada == NULL){

        return NULL;

    }

    Memoria *novoElementoListaReferencia = malloc(sizeof(Memoria));

    if(novoElementoListaReferencia == NULL){

        free(novaMemoriaAlocada);
        return NULL;

    }

    novoElementoListaReferencia->endereco = novaMemoriaAlocada;
    novoElementoListaReferencia->contadorRef = 1;
    novoElementoListaReferencia->prox = NULL;

    if(listaReferencia == NULL){

        listaReferencia = novoElementoListaReferencia;

    }else{

        Memoria *aux = listaReferencia;

        while (aux->prox != NULL){

            aux = aux->prox;
        }

        aux->prox = novoElementoListaReferencia;
    }

    return novaMemoriaAlocada;
}

void atrib2(int **destino, void *origem){

    if(*destino != NULL){

        Memoria *correrLista = listaReferencia;

        while (correrLista != NULL && correrLista->endereco != *destino){

            correrLista = correrLista->prox;

        }
     
    }

    if(origem != NULL){

        Memoria *elementoOrigem = listaReferencia;

        while (elementoOrigem != NULL && elementoOrigem->endereco != origem){

            elementoOrigem = elementoOrigem->prox;

        }

        if(elementoOrigem == NULL){

            elementoOrigem = malloc(sizeof(Memoria));
            elementoOrigem->endereco = origem;
            elementoOrigem->contadorRef = 1;
            elementoOrigem->prox = listaReferencia;
            listaReferencia = elementoOrigem;

        }else{

            elementoOrigem->contadorRef++;

        }

        (*destino) = origem;

    }else{

        (*destino) = NULL;

    }
}

void limparListaReferencia(){

    Memoria *aux = listaReferencia;

    while(aux != NULL){

        free(aux->endereco);
        free(aux);
        aux = aux->prox;

    }
}
