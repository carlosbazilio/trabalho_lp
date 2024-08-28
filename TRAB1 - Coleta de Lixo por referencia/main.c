#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "coletalixo.h"

int main(){

    int* v = malloc2(sizeof(int));
    *v = 10;
    int* w = malloc2(sizeof(int));
    dump();
    *w = 20;
    atrib((void**)&v, w);
    dump();
    char *c = malloc2(sizeof(char));
    *c = 'Z';
    dump();
    atrib((void**)&w, NULL);
    atrib((void**)&c, NULL);

    dump();
    return 0;
}