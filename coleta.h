#ifndef COLETA_H
#define COLETA_H

void* malloc2(int tamanho);

void dump();

void _atrib_real(void** a, void* b);

#define atrib2(dest, src) _atrib_real((void**)&(dest), (void*)(src))


#endif
