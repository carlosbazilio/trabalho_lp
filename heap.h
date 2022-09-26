#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED


typedef struct heap {
  void * endereco;
  int cont_ref;
  struct heap  *next;
}heap;

extern heap* head;

void* malloc2(int size);

void atrib2(void** pointer1, void* pointer2 );

void dump();

void PrintHeap();

#endif // HEAP_H_INCLUDED
