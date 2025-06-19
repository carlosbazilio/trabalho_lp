#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Objeto
{
    bool marcado;
    int dado;
    struct Objeto *prox;
    struct Objeto *ref;
} objeto;

objeto *head = NULL;
objeto *root = NULL;
objeto *roots[10];
int contRoot = 0;

void adicionaRoot(objeto *obj)
{
    if (contRoot < 10)
    {
        roots[contRoot++] = obj;
    }
}

objeto* ObjetoNovo(int valor)
{
    objeto *novo = (objeto*)malloc(sizeof(objeto));

    novo->marcado = false;
    novo->ref = NULL;
    novo->dado = valor;
    novo->prox = head;

    head = novo;
    return novo;
}

void percorre()
{
    objeto *atual = head;

    while (atual != NULL)
    {
        printf("\nValor: %d, marcado: %d\n", atual->dado, atual->marcado);
        atual = atual->prox;
    }
}

void marcar(objeto *obj)
{
    if (obj == NULL || obj->marcado)
        return;

    obj->marcado = 1;

    marcar(obj->ref);
}

void marcacao()
{
    for (int i = 0; i != contRoot; i++)
    {
        marcar(roots[i]);
    }
}
void varre()
{

    objeto **atual = &head;

    while (*atual != NULL)
    {
        if (!(*atual)->marcado)
        {
            objeto *aux = *atual;
            (*atual) = aux->prox;
            free(aux);
        }
        else
        {
            (*atual)->marcado = false;
            atual = &(*atual)->prox;
        }
    }
}

void MarkAndSweep()
{
    
    marcacao();
    
    varre();
}

void printObj()
{
    printf("Objetos ativos:\n");
    objeto *atual = head;
    while (atual != NULL)
    {
        printf("Objeto %p (valor: %d, marcado: %s)\n",(void *)atual,atual->dado,atual->marcado ? "sim" : "não");
        atual = atual->prox;
    }
    printf("\n");
}

int main()
{

objeto* obj1 = ObjetoNovo(10);
objeto* obj2 = ObjetoNovo(20);
objeto* obj3 = ObjetoNovo(30);
objeto* obj4 = ObjetoNovo(40);

obj1->ref = obj2;
obj2->ref = obj3;
obj3->ref = obj4;

adicionaRoot(obj1);

printf("\n Antes da coleta:\n");

printObj();
MarkAndSweep();

printf("\n Depois da coleta:\n");
printObj();


obj2->ref = NULL;  
MarkAndSweep();
printObj();

    return 0;
}
