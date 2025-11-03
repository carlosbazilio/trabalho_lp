#include <stdio.h>
#include "coleta.h"

//main usando macro

int main() {
    printf("1. Alocando 'v' (int*)...\n");
    int* v = malloc2(sizeof(int));
    *v = 10;

    printf("2. Alocando 'w' (int*)...\n");
    int* w = malloc2(sizeof(int));
    *w = 20;

    printf("\n--- POS MALLOCS ---\n");
    dump();


    printf("\n3. Executando atrib2(v, w)\n");
    atrib2(v, w);

    printf("\n---APOS ATRIB2---\n");
    dump();


    printf("\n4. Alocando 'c' (char*)...\n");
    char* c = (char*)malloc2(sizeof(char));
    *c = 'Z';

    printf("\n--- DUMP 3: APÓS ALOCAR 'c' ---\n");
    dump();

    // ---

    printf("\n5. Executando atrib2(w, NULL)...\n");
    atrib2(w, NULL);

    printf("\n--- DUMP 4: APÓS w = NULL ---\n");
    dump();


    printf("\n6. Executando atrib2(v, NULL)...\n");
    atrib2(v, NULL);

    printf("\n--- DUMP 5: APÓS v = NULL ---\n");
    dump();

    printf("\n7. Executando atrib2(c, NULL)...\n");
    atrib2(c, NULL);

    printf("\n--- DUMP 6: FINAL ---\n");
    dump();


    printf("\n--- TESTE CONCLUÍDO ---\n");
    return 0;
}
