#ifndef _CONTREF_H_
#define _CONTREF_H_


#include <stdio.h>
#include <stdlib.h>


typedef struct lista{

    int status;
    void* endereco;
    struct lista* prox;

}Lista;

Lista* head = NULL;

void insert(Lista** head, Lista* body){

    Lista* aux = (Lista*)malloc(sizeof(Lista));

    if(*head == NULL){
        *head = body;
    }
    else{
        aux = *head;
        while(aux->prox){
            aux = aux->prox;
        }
        aux->prox = body;
    }
    return;

}


Lista* coleta(Lista* body){

    Lista* aux, *remover;

    if(body){
        if(body->status == 0){
            remover = coleta(body->prox);
            free(body);
            return remover;
        }
        else{
            aux = body;
            aux->prox = coleta(aux->prox);
            return aux;
        }
    }

    return NULL;
}


void* malloc2(int tam){


    Lista* body = (Lista*)malloc(sizeof(Lista));
    body->status = 1;
    body->prox = NULL;
    body->endereco = malloc(tam);

    insert(&head, body);

    return body->endereco;


}

void arranjo(void* endereco1, void* endereco2){

    Lista* aux = head;

    while(aux){
        if(aux->endereco == endereco1){
            aux->status = aux->status - 1;
        }
        if(endereco2){
            if(aux->endereco == endereco2){
                aux->status = aux->status + 1;
            }
        }
        aux = aux->prox;
    }

}

void atrib2(void* endereco1, void* endereco2){

    arranjo(endereco1, endereco2);
    endereco1 = endereco2;
    head = coleta(head);
    return;

}

void dump(){

    Lista* aux = head;
    printf("\n/----------------------------------------------------------------------------------/\n\n");
    while(aux){
        
        printf("Endereco: %p, Contagem de Referencias: %d.\n", aux->endereco, aux->status);
        aux = aux->prox;
    }

}

#endif // _CONTREF_H_

