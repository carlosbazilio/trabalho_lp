#include "ref_count.h"
#include <stdlib.h>

typedef struct metadata {
    int ref_count;     // Contador de referências
    size_t size;       // Tamanho do bloco alocado (para debug)
} Metadata;

void* malloc2(size_t size) {
    // Aloca memória para metadados + dados do usuário
    Metadata* meta = (Metadata*)malloc(sizeof(Metadata) + size);
    if (!meta) return NULL;  // Falha na alocação

    meta->ref_count = 1;  // Inicializa contador
    meta->size = size;

    // Retorna endereço após os metadados (área do usuário)
    return (void*)(meta + 1);
}

void atrib2(void** dest, void* src) {
    
    if (*dest) {
        Metadata* old_meta = (Metadata*)((char*)(*dest) - sizeof(Metadata));
        old_meta->ref_count--;

        
        if (old_meta->ref_count == 0) {
            free(old_meta);
        }
    }

    *dest = src;

    if (src) {
        Metadata* new_meta = (Metadata*)((char*)src - sizeof(Metadata));
        new_meta->ref_count++;
    }
}