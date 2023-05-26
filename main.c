#include "contadorReferencia.h"

int main(){

    int *v = malloc2(sizeof(int));
    *v = 10;
    int *w = malloc2(sizeof(int));
    dump();
    *w = 20;
    atrib2(v, w);
    dump();
    char *c = malloc2(sizeof(char));
    *c = 'Z';
    dump();
    atrib2(w, NULL);
    dump();

    printf("\n/----------------------------------------------------------------------------------/\n");
    return 0;
}