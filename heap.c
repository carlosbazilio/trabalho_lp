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
heap *addbest(heap *h, heap *novo){
    heap *a,*p;// p: proximo
    char id_m;//id do menor desperdicio
    int m_dist= 2*Theap; //maior distancia possivel
    int dist, p_ini,a_ini,a_tam;
    a=h;
    if(a!=NULL){//há algo no heap?      
        p=a->prox;
        if(p==NULL){//segunda iteração
            novo->item->inicio= (a->item->inicio)+(a->item->tamanho);
            a->prox =novo;

            return a;
        }
    }
    else{//primeira itereção
        p=NULL;   
        novo->prox = NULL;
        novo->item->inicio=0;
        return novo; 
    }
    while (p!=NULL)//enquanto existir prox 
    {
        p_ini = p->item->inicio;
        a_ini = a->item->inicio;
        a_tam = a->item->tamanho;
        dist = p_ini-(a_ini+a_tam);//espaço entre o atual e o prox
        if((dist - novo->item->tamanho)==0)//verifica espaço entre os elementos| faz o -1?
        {
            //inseri
            novo->item->inicio = (a->item->inicio)+(a->item->tamanho);            
            novo->prox = a->prox;
            a->prox= novo;
            return h;//'a' inicia como um vetor copia de h entao as alteraçoes de a valem em h
        }
        else{
            if(dist<m_dist && dist > 0){
                m_dist=dist;
                id_m = a->item->id;
            }else{                
            }
        }

        a=p;
        
        p=a->prox;
      
    }
    if(m_dist>Theap){//se maior n há espaço entre eles
        novo->item->inicio = (a->item->inicio)+(a->item->tamanho);
        a->prox=novo;
        return h;
    }else{//se menor ha algum espaço com menor desperdicio
        a=h;
        while(a!=NULL){
            if(a->item->id==id_m){
                p = a->prox;
                novo->item->inicio = (a->item->inicio)+(a->item->tamanho);
                a->prox =novo;
                novo->prox =p;
                return h;
            }
        }

    }
    
            
           
}
heap *addworst(heap *h, heap *novo){
     heap *a,*p;// p: proximo
    char id_m;//id do menor desperdicio
    int m_dist= 0; //maior distancia possivel
    int dist, p_ini,a_ini,a_tam;
    a=h;
    if(a!=NULL){//há algo no heap?      
        p=a->prox;
        if(p==NULL){//segunda iteração
            novo->item->inicio= (a->item->inicio)+(a->item->tamanho);
            a->prox =novo;

            return a;
        }
    }
    else{//primeira itereção
        p=NULL;   
        novo->prox = NULL;
        novo->item->inicio=0;
        return novo; 
    }
    while (p!=NULL)//enquanto existir prox 
    {
        p_ini = p->item->inicio;
        a_ini = a->item->inicio;
        a_tam = a->item->tamanho;
        dist = p_ini-(a_ini+a_tam);//espaço entre o atual e o prox
        if((dist - novo->item->tamanho)==0)//verifica espaço entre os elementos| faz o -1?
        {
            //inseri
            novo->item->inicio = (a->item->inicio)+(a->item->tamanho);            
            novo->prox = a->prox;
            a->prox= novo;
            return h;//'a' inicia como um vetor copia de h entao as alteraçoes de a valem em h
        }
        else{
            if(dist>m_dist && dist > 0){
                m_dist=dist;
                id_m = a->item->id;
            }else{                
            }
        }
        a=p;        
        p=a->prox;
      
    }
    if(m_dist<Theap){//se esta dentro do heap
        novo->item->inicio = (a->item->inicio)+(a->item->tamanho);
        a->prox=novo;
        return h;
    }else{//se menor ha algum espaço com menor desperdicio
        a=h;
        while(a!=NULL){
            if(a->item->id==id_m){
                p = a->prox;
                novo->item->inicio = (a->item->inicio)+(a->item->tamanho);
                a->prox =novo;
                novo->prox =p;
                return h;
            }
        }

    }
    

}
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
    heap *p,*aux,*aux2;//a- atual p-proximo
    if(a==NULL){
        novo->item->inicio=0;
        return novo;

    }else{
        if(a->prox!=NULL){
            p=a->prox;
        }else{
            novo->item->inicio=(a->item->inicio)+(a->item->tamanho);
            a->prox=novo;
            return a;
        }
            
    }    
    aux = a;   
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
        printf("\nID: %c Tamanho Alocado:%d Inicio:%d",h->item->id,h->item->tamanho,h->item->inicio);
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
        novo->inicio =-1;
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
                break;
            case 3://best
                h = addbest(h,No);
                break;
            case 4://worse
             h = addworst(h,No);
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

    if(h!=NULL && Finderid(h,id)){   
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
}
heap *chamadelete(heap *h, char id){
    if(Finderid(h,id)){
        return delete(h,id);
    }
    return h;
}

void simula(){
 heap *HEAP = NULL;
    
    printf("\nadd a 10\n");
    HEAP = add(HEAP,'a',10);
    imprimirHeap(HEAP);
    printf("\nadd b 5\n");
    HEAP = add(HEAP,'b',5);
    imprimirHeap(HEAP);  
    printf("\nadd c 10\n");
    HEAP = add(HEAP,'c',10);
    printf("\nadd a 10\n");
    HEAP = add(HEAP,'a',10);
    printf("\nadd f 10\n");
    HEAP = add(HEAP,'f',10);
    printf("\nadd d 10\n");
    HEAP = add(HEAP,'d',10);
    printf("\nHEAP:\n");
    imprimirHeap(HEAP);
    printf("\nDelete b\n");
    delete(HEAP,'b');
    printf("\nDelete f\n");
    delete(HEAP,'f');
    imprimirHeap(HEAP);
    printf("\nadd j 10\n");
    HEAP = add(HEAP,'j',10);
    printf("\nadd k 1\n");
    HEAP = add(HEAP,'k',1);
    imprimirHeap(HEAP);
    destroi(HEAP);
   
}

int main(){
   //printf("\nfirst:\n");
  // escolhido =1;
   //simula();
   printf("\nfit:\n");
   escolhido =2;
   simula();
  /* printf("\n\n\nbest:\n");
   escolhido =3;
   simula();
   printf("\n\n\nworst:\n");
   escolhido =4;
   simula();
*/
    return 0;
}
