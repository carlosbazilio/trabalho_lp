/*  --------------------- COMPILAR ---------------------

    1) gcc -c main.c -o main.o
    2) gcc -o main main.o -L. -l_coletor

*/

#include <stdio.h>
#include <stdlib.h>
#include "coletor.h"

int main ()
{
    dump();

    int* a = malloc2(sizeof(int));
    int* b = malloc2(sizeof(int));
    int* c = malloc2(sizeof(int));
    int* d = malloc2(sizeof(int));
    int* e = malloc2(sizeof(int));

    dump();

    *a = 15;
    *b = 20;
    *c = 25;
    *d = 30;
    *e = 35;

    dump();

    printf("e = %d\n",*e);
    atrib2(&e,a);
    printf("e = %d\n",*e);

    dump();

    printf("b = %d\n",*b);
    atrib2(&b,NULL);

    dump();

    printf("c = %d\n",*c);
    printf("d = %d\n",*d);

    atrib2(&c,a);
    atrib2(&d,a);

    dump();

    atrib2(&a,NULL);

    dump();

    printf("c = %d\n",*c);
    printf("d = %d\n",*d);

    return 0;
}