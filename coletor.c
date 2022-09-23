#include <stdio.h>
#include <stdlib.h>

typedef struct contador_ref {
    int contador;
    void* endereco;
    struct contador_ref* prox;
} Contador;

Contador* memoria = NULL;

void* malloc2(int tamanho)
{   
    Contador* novo_contador = (Contador*)malloc(sizeof(Contador));
    novo_contador->contador = 1;

    void* novo_endereco = malloc(tamanho);
    novo_contador->endereco = novo_endereco;

    if (memoria == NULL)
    {
        novo_contador->prox = memoria;
        memoria = novo_contador;
    }

    else 
    {
        Contador* aux = memoria;

        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }

        aux->prox = novo_contador;
        novo_contador->prox = NULL;
    }

    return novo_endereco;
}

void atrib2(void** endereco1, void* endereco2)
{
    Contador* aux = memoria;
    Contador* anterior = NULL;
    int i = 0;

    while (i != 2)
    {
        if (aux->endereco == *endereco1)
        {
            aux->contador -= 1;
            ++i;

            if (aux->contador == 0)
            {
                free(aux->endereco);

                if (anterior == NULL)
                {
                    memoria = aux->prox;
                    free(aux);
                }

                else
                {
                    anterior->prox = aux->prox;
                    free(aux);
                    aux = anterior;
                }
            }

            if (endereco2 == NULL)
            {
                break;
            }
        }

        else if (aux->endereco == endereco2)
        {
            aux->contador += 1;
            ++i;
        }
        
        anterior = aux;
        aux = aux->prox;
    }

    *endereco1 = endereco2;
}

void dump()
{
    if (memoria == NULL)
    {
        printf("\n-----------------MEMORIA VAZIA-----------------\n\n");
    }

    else
    {
        Contador* aux = memoria;
        
        printf("\n-----------------MEMORIA-----------------");
        while (aux != NULL)
        {
            printf("\nEndereço: %p",aux->endereco);
            printf("\nConteúdo: %d",*((int*)aux->endereco));
            printf("\nContador: %d\n",aux->contador);
            aux = aux->prox;
        }
        printf("-----------------------------------------\n\n");
    }
}