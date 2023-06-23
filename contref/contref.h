#ifndef _CONTREF_H
#define _CONTREF_H

typedef struct Memoria {
    void *endereco;
    int contadorRef;
    struct Memoria *prox;
} Memoria;

void imprimirListaReferencia(); 
void atualizarListaReferencia(Memoria *elemento,int contador); 
void *malloc2(int tamanho); 
void atrib2(int **destino, void *origem);
void limparListaReferencia(); 

#endif 