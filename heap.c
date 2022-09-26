#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

heap* head= NULL;

void* malloc2(int size){

	heap *bloco = (heap *)malloc(sizeof(heap));

	bloco->endereco = malloc(sizeof(size));
	bloco->cont_ref = 1;
	bloco->next = NULL;

	bloco->next = head;
	head = bloco;

	return head->endereco;

}

void atrib2(void** pointer1, void* pointer2 ){

	heap *aux = head;

	while (aux != NULL) {

        if (aux->endereco == *pointer1){

        	aux->cont_ref -=1;
        }

        if(aux->endereco == pointer2 ){
        	aux->cont_ref += 1;
        }
        aux = aux->next;
	}

	*pointer1 = pointer2;

}

void dump(){

	heap *aux = head;
	heap *ant;

    while (aux != NULL && aux->cont_ref == 0) {

        head = aux->next;
        free(aux);
        aux = head;

    }
    while (aux != NULL) {

        while (aux != NULL && aux->cont_ref != 0) {

            ant = aux;
            aux = aux->next;
       }
        if (aux== NULL){
            return;
        }

        ant->next = aux->next;

        free(aux);

        aux = ant->next;

    }
}

void PrintHeap(){
    heap *aux;

    if(head==NULL){
        printf("Heap está vazia");
        return;
    }else{
        aux = head;
        while(aux !=NULL){
            printf("*******************\n");
            printf("* %d *             *\n",aux->cont_ref);
            printf("*****             *\n");
            printf("*                 *\n");
            printf("*                 *\n");
            printf("*  %p *\n",aux->endereco);
            printf("*******************\n");

            aux=aux->next;
        }
    }
}
