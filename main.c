#include <stdio.h>
#include <stdlib.h>
#include "heap.h"


int main()
{
int *v = malloc2(sizeof(int));
    *v = 10;
    printf("\n1 - Print\n");

    PrintHeap();
    int *w = malloc2(sizeof(int));
    printf("\n2 - Print\n");

    PrintHeap();
    dump();
    printf("\n3 - Print\n");

    PrintHeap();
    atrib2(&v,w);
    printf("\n4 - Print\n");

    PrintHeap();
    dump();
    printf("\n5 - Print\n");

    PrintHeap();
    char *c = malloc2(sizeof(char));
    *c = 'Z';
    printf("\n6 - Print\n" );

    PrintHeap();
    dump();
    printf("\n7 - Print\n");

    PrintHeap();
    atrib2(w, NULL);
    printf("\n8 - Print\n");

    PrintHeap();
    dump();
    printf("\n9 - Print\n");

    PrintHeap();


    return 0;

}
