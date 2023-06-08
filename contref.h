#include <stdio.h>
#include <stdlib.h>

typedef struct contador{
    int cont;
    void *endereco;
    struct heap *prox;
    struct heap *ant;
}contador;

contador *removeE(contador *a, void *endereco);
void incrementa(void *b2, int n);
void *malloc2(int size);
void atrib2(void *b1, void *b2);
void dump();