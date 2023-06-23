#include <stdio.h>
#include <stdlib.h>
#include "contref/contref.h"

int main()
{
    int *v=malloc2(sizeof(int));
    *v=10;
    int *w=malloc2(sizeof(int));
    *w=20;
    atrib2(&v,w);
    char *c=malloc2(sizeof(char));
    *c='z';
    atrib2(&w,NULL);
    imprimirListaReferencia();
    limparListaReferencia();
    return 0;
}

// EXECUTAR O garbabecollector.c
// gcc -o garbagecollector garbagecollector.c -I./contref -L./contref -lcontref -Wl,-rpath=./contref

// COMPILAR A BIBLIOTECA contref.c
// gcc -c -o contref.o contref.c -fPIC

// CRIANDO A BIBLIOTECA
// gcc -shared -o libcontref.so contref.o

