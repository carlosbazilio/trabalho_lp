#ifndef CDL_REFCOUNT_H
#define CDL_REFCOUNT_H

#include <stddef.h>

void* malloc2(size_t tamanho);
void atrib2(void** destino, void* origem);
void CdL_status(void);
void CdL_limpar(void);

#endif
