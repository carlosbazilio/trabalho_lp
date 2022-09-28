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
heap *addFirst(heap *h,heap *novo){
    heap *r,*atual,*p;bool add=false;
    if(h==NULL){
        novo->item->inicio=0;
        return novo;
    }else if(h->prox==NULL){
        novo->item->inicio=(h->item->inicio)+(h->item->tamanho);
        h->prox=novo;
        return h;
    }else{
        r=h;
      
        while (h->prox!=NULL)
        {
          if(((h->prox->item->inicio)-(h->item->inicio+h->item->tamanho))>(novo->item->tamanho)-1){
            novo->prox=h->prox;
            novo->item->inicio=(h->item->inicio+h->item->tamanho);
            h->prox=novo;
            add=true;
            break;

          }
          h=h->prox;
          
          
        }
        if(!add){
            novo->item->inicio=(h->item->inicio)+(h->item->tamanho);
            novo->prox=NULL;
            h->prox=novo;
        }
        

        return r;
    }

    
    return h;
    
}

heap *addFit(heap *a, heap *novo, int cont){
    heap *aux,*p,*aux2;
    bool add = false;
    aux=a;
    if(a==NULL){
        novo->item->inicio = 0;
        novo->prox=NULL;
        return novo;

    }else if(a->prox == NULL){
        novo->item->inicio =a->item->tamanho;
        novo->prox=NULL;
        a->prox = novo;
        return a;
    }else{
        /*
        if((novo->item->tamanho)-(a->item->inicio)==0){
            novo->item->inicio = 0;
            novo->prox=a;
            return novo;
        }
        if(a->prox==NULL){
            novo->item->inicio=a->item->tamanho;
            novo->prox=NULL;
            a->prox=novo;
            return a;
        }
        else{
            p=a->prox;                
            while (p!=NULL)
            {
               if((novo->item->tamanho)-(a->item->inicio)==0){
                    novo->item->inicio = 0;
                    novo->prox=a;
                    return aux;
                }
                a = p;
                if(a->prox != NULL)
                {
                    p = a->prox;                
                }
                else{
                    p=NULL;
                }
            }
            return addFirst(aux,novo);
            
        }
        */
       while(a->prox!=NULL){
            p = a->prox;
            if(novo->item->tamanho==(p->item->inicio)-(a->item->tamanho)+(a->item->inicio)){
                novo->prox=p;
                a->prox=novo;
                novo->item->inicio=(a->item->tamanho)+(a->item->inicio);               
                add= true;
                break;
            }
            a=p;
       }
    }
    if(!add){        
        aux = addFirst(aux,novo);
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
                h = addFit(h,No,0);
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
    if(h!=NULL){ 
          
        if(h->item->id==id){

            if(h->prox !=NULL)
            {
                heap *aux = h->prox;
                free(h->item);
                return aux;
            }
            else
            {
                free(h->item);
                h->item=NULL;
                h->prox=NULL;
                return h;
            }
        }    
        else
        {
            h->prox = delete(h->prox,id);
            return h;
        }
    }
    return h;
}
heap *chamadelete(heap *h, char id){
    if(Finderid(h,id)){
        return delete(h,id);
    }
    return h;
}

void simula(){
 heap *HEAP = NULL;
    
    printf("\nadd a 10");
    HEAP = add(HEAP,'a',10); //add  a+{}      

    printf("\nadd b 5");
    HEAP = add(HEAP,'b',5);//add  b+{a}   

    printf("\nadd c 10");
    HEAP = add(HEAP,'c',10);//add  c+{a,b}
    
    printf("\nadd a 10");
    HEAP = add(HEAP,'a',10);//add  a+{a,b,c}//não entra

    printf("\nadd f 10");    
    HEAP = add(HEAP,'f',10);//add  f+{a,b,c}

    printf("\nadd d 10");
    HEAP = add(HEAP,'d',10);//add  d+{a,b,c,f}    

    printf("\nDelete b");
    HEAP = delete(HEAP,'b');//delete {a,5,c,f,d}

    printf("\nDelete c");
    HEAP = delete(HEAP,'c');//delete {a,15,f,d} 
    
    printf("\nHEAP:\n");   
    imprimirHeap(HEAP);

    printf("\nadd j 10");
    HEAP = add(HEAP,'j',10);//add j {a,j,5,f,d}

    printf("\nadd k 5");
    HEAP = add(HEAP,'k',5);//add{a,j,k,f,d}

    printf("\nHEAP:\n");   
    imprimirHeap(HEAP);
    destroi(HEAP);
   
}

int main(){
   printf("\nfirst:\n");
   escolhido =1;
   simula();
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
