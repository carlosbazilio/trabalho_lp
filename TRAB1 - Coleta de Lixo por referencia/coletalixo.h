#ifndef COLETALIXO_H
#define COLETALIXO_H

#include <stddef.h>

//Aloca memoria e cria um novo bloco com contador de referencias
void* malloc2(size_t tamanho);

// Função para atribuir ponteiros e atualizar contagem de referencias
void atrib(void** ptr1, void *ptr2);

//Imprime mudanças
void dump();

#endif