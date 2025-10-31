#include <stdio.h>
#include "CdL_refcount.h"

int main() {
    int* p = (int*) malloc2(sizeof(int));
    *p = 10;

    int* q = NULL;
    atrib2((void**)&q, p); // q aponta para p (ref +1)

    printf("Valor: %d\n", *q);

    atrib2((void**)&p, NULL); // libera p (ref -1)
    CdL_status();

    atrib2((void**)&q, NULL); // libera q também

    CdL_limpar(); // limpa tudo que restou
    return 0;
}
