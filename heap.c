#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
/*
    Ricardo Menezes G. P. Bandeira
    João Victor de M. Freitas
    HEAP 
*/
#define Theap 80//tamanho do heap
int escolhido=1;//1 first 2 fit 3 best 4 worst
typedef struct elemento{
    char id;//identificador
    int tamanho;//tamanho na memoria
    int inicio;//ponto de partida na memoria
}elemento;
typedef struct heap{
    elemento *item;
    struct heap *prox;
    
}heap;


heap *addFirst(heap *atual,heap *novo){  
    int A_ini,A_tam,N_tam;
    if(atual != NULL)
    {   
        A_ini=(atual->item->inicio);
        A_tam=(atual->item->tamanho);
        if(atual->prox==NULL){//se não há prox       
           
            N_tam=novo->item->tamanho;
            if(A_ini+A_tam+N_tam<Theap ){           
                novo->item->inicio= A_ini+A_tam;//o inicio do atual mais o tamanho
                atual->prox= novo;
                
            }
            else{
                return NULL;//nao ha espaço
            }

        }
        else//se ha prox
        {
            if((atual->item->inicio)+(atual->item->tamanho)+(novo->item->tamanho)-1 < (atual->prox->item->inicio)){
                novo->prox = atual->prox;
                novo->item->inicio = (atual->item->inicio)+(atual->item->tamanho);
                atual->prox = novo;             
                return atual;
            }else{
                atual->prox=addFirst(atual->prox,novo);
                return atual;
            }
         
            
        }
    }
    else
    {       
        if(novo->item->tamanho<Theap )
        {//se item cabe no heap
        novo->item->inicio=0; 
        novo->prox=NULL;
        return novo;
        }           
    }

    return atual;
    
}
heap *addFit(heap *a, heap *novo){
    bool inseriu = false;
    heap *p,*aux,*aux2;//a- atual p-proximo
    if(a==NULL){
        p=NULL;
    }else{
        if(a->prox!=NULL)
            p=a->prox;
    }
    
    aux = a;
    int n_tam,a_ini,a_tam, p_ini;
    while(!inseriu){
        if((a_ini+a_tam)<Theap){
            if(a==NULL){
                a=novo;
                return a;
            }
            if(p==NULL){
                a->prox=novo;
                return a;
            }
            n_tam=(novo->item->tamanho);
            a_ini=(a->item->inicio);
            a_tam=(a->item->tamanho);
            p_ini=(p->item->inicio);
            if(n_tam-((a_ini+a_tam)-p_ini)==0){
                //inseri
                novo->prox=p;
                a->prox=novo;
                inseriu =true;
            }
            aux2=p->prox;
            a=p;
            p=aux2;
        } else{
            return addFirst(aux,novo);
        }
    }
    return aux;
}
void modo(){
    int a = 0;
    printf("\nEscolha o modo de inserção no HEAP\n");
    printf("\n1- First\n");
    printf("\n2- Fit\n");
    printf("\n3- Best\n");
    printf("\n4- Worst\n");
    scanf("%d",&a);
    switch(a){
        case 1://first
            escolhido = 1;
            break;
        case 2://fit
            escolhido = 2;
            break;
        case 3://best
            escolhido = 3;
            break;
        case 4://worst
            escolhido = 4;
            break;
        default:
            printf("\nmodo inválido\n");
            modo();
            break;
    }
}

void imprimirHeap(heap *h){
    if(h==NULL){
        printf("\n!NULL!\n");
    }
    while(h!=NULL){        
        printf("\nID: %c Tamanho:%d Inicio:%d",h->item->id,h->item->tamanho,h->item->inicio);
        h=h->prox;
    }
}
bool Finderid(heap *h,char id){
    while(h!=NULL){
        if(h->item->id == id){
            return false;
        }
        h=h->prox;
    }
   
    return true;
}
heap *add(heap *h,char id,int tam){  

    if(Finderid(h,id)){       
        elemento *novo;//novo elemento 
        novo = malloc(sizeof(elemento));
        novo->id = id;
        novo->tamanho=tam;
        
        heap *No;//novo nó da lista
        No = malloc(sizeof(heap));
        No->prox=NULL;
        No->item=novo;

        switch (escolhido){
            case 1://first
                h = addFirst(h,No);
                break;
            case 2://fit
                h = addFit(h,No);
            case 3://best
                break;
            case 4://worse
                break;
            
        }    
        return h;//retorna alterada
    }
    else{
        return h;//retorna sem alteração
    }
}
void destroi(heap *h){
    heap *aux;
    if(h!=NULL){
        while (aux!=NULL)
        {
            aux = h->prox;
            free(h->item);
            free(h);
            h = aux;
        }    
    }
    
    
}

heap *delete (heap *h, char id){
    if(h->item->id==id){
        if(h->prox !=NULL){
            heap *aux = h->prox;
            free(h->item);
            h->item=aux->item;
            h->prox=aux->prox;
            return h;
        }else{
            free(h->item);
            h->item=NULL;
            h->prox=NULL;
            return h;
        }
    }    
    else{
        return delete(h->prox,id);
    }
}

heap *chamadelete(heap *h, char id){
    if(Finderid(h,id)){
        return delete(h,id);
    }
    return h;
}

heap *addbest(heap *h, heap *novo){
    //menor desperdicio
    //Dist = P_ini - A_ini+A_tam
    //if(Dist==0){add}
    //if(Dist<menorD){menorD=Dist; M_pos=A_ini+A_tam}
}
heap *addworst(){

}
int main(){
    heap *HEAP = NULL;
    escolhido = 2;
    printf("add a 8\n");
    HEAP = add(HEAP,'a',8);    
    printf("add b 9\n");
    HEAP = add(HEAP,'b',7);
    printf("add c 9\n");
    HEAP = add(HEAP,'c',9);
    printf("add a 10\n");
    HEAP = add(HEAP,'a',10);
    printf("add f 11\n");
    HEAP = add(HEAP,'f',11);
    imprimirHeap(HEAP);
    printf("\nDelete b\n");
    delete(HEAP,'b');
    HEAP = add(HEAP,'j',5);
    HEAP = add(HEAP,'k',2);
    imprimirHeap(HEAP);
    destroi(HEAP);
   
    return 0;
}
