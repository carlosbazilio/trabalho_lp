#include "ref_count.h"
#include <stdio.h>

int main() {
    int* a = (int*)malloc2(sizeof(int));
    *a = 42;

    int* b = NULL;
    atrib2((void**)&b, a);  

    atrib2((void**)&a, NULL); 
    atrib2((void**)&b, NULL); 

    return 0;
}