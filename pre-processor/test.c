#include "stdio.h"
#include "my_lib.h"

// typedef struct test
// {
//     int info;
// }Test;


// this is some cool comment

int func_basica(int x, int y){
    return x+y;
}

int  main(){
    printf("2^2", pow(2,2));
    printf("This is a test\n");
    int x = 2;
    int y = 3;
    printf("soma = %d\n", func_basica(x, y)); // I'll need some comments like this
    float f = 2.5;
    printf("resultado 2.5 * 5: %f", 2.5*5);

    // Test *variavel = (Test*) malloc(sizeof(Test));
    // variavel->info = 5;
    // printf("a: %d", variavel->info);
}