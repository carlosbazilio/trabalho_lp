#include <stdio.h>
#include <stdlib.h>
#include "contagem.h"

int main() {
    int *v = NULL; 
    int *w = NULL;
    char *c = NULL;

    v = gc_aloca(sizeof(int));
    if (v != NULL) *v = 100;
    
    w = gc_aloca(sizeof(int));
    if (w != NULL) *w = 200;

    gc_status_memoria(); 

    gc_atribui((void **)&v, w);
    if (v != NULL) printf("Valor de v: %d\n", *v);

    gc_status_memoria(); 

    c = gc_aloca(sizeof(char));
    if (c != NULL) *c = 'k';

    gc_status_memoria(); 

    gc_atribui((void **)&w, NULL);

    gc_status_memoria(); 

    gc_atribui((void **)&v, NULL);

    gc_status_memoria(); 

    gc_atribui((void **)&c, NULL);
    
    gc_limpeza_final(); 

    return 0;
}