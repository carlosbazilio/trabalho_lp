#ifndef GC_REFCOUNT_H
#define GC_REFCOUNT_H

#include <stddef.h>

void *gc_aloca(size_t tamanho);

void gc_atribui(void **ponteiro_destino, void *ponteiro_origem);

void gc_status_memoria();

void gc_limpeza_final();

#endif 