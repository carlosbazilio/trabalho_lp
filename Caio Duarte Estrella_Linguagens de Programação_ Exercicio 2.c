#include <stdio.h>
#include <stdlib.h>
#define TAM 30
typedef struct 
{
	int id;
	int qnt;
	int end;	
}memoria;
memoria v[TAM];


int Medir_espaco_First(int vet[TAM], int qnt)
{
	int aux=0;
	for(int i=0; i<TAM; i++)
	{
			if(vet[i]==0)
			{
				aux++;
			}
			else
			{
				aux=0;
			}
			if(aux==qnt)
			{
				aux=i-aux;
				return aux;
			}
	}
}


void First_Fit (int vet[TAM], int qnt, int x,int *j)
{
	int cont=0;
	cont=Medir_espaco_First(vet,qnt)+1;
	*j=cont;
	for(int i=cont; i<cont+qnt; i++)
	{
		vet[i]=1;
	}
}


void Exibir(int vet[TAM],int c)
{
	printf("\n\n\n");
	for(int t=0; t<c; t++)
	{
		if(v[t].id!=0)
		{
			printf("O id: %d esta alocado a partir do endereco: %d \n",v[t].id,v[t].end);
		}
	}
	
	printf("\n\n");
	
	for(int i=0; i<TAM; i++)
	{
		printf("%d  ",vet[i]);
	}
	printf("\n\n\n");
}


void Delete (int vet[TAM], int ide)
{
	for(int j=0; j<TAM; j++)
	{
		if(ide==v[j].id)
		{
			for(int i=v[j].end; i<v[j].end+v[j].qnt; i++)
			{
				vet[i]=0;
			}
			v[j].id=0;
		}
	}
}


void Atribuir(int vet[TAM], int id1, int id2 )
{
	int aux;
	for (int i=0; i<TAM; i++)
	{
		if(v[i].id==id1)
		{
			for(int g=0; g<TAM; g++)
			{
				if(v[g].id==id2)
				{
					aux=v[i].id;
					Delete(vet, v[i].id);
					v[i].end=v[g].end;
					v[i].qnt=v[g].qnt;
					v[i].id=aux;
				}
			}
		}
	}	
}


void Last_fit(int vet[TAM], int qnt, int x,int *j,int c)
{
	int h;
	int cont=0;
	for(int i=TAM-1; i>=0; i--)
	{
		if(vet[i]==1 && c!=0)
		{
			for(h=i+1; h<i+qnt+1; h++)
			{
				vet[h]=1;
			}
			i=-1;
			*j=h-qnt;
		}
		else
		if(vet[0]==0 && c==0)
		{
			for(h=0; h<qnt; h++)
			{
				vet[h]=1;
			}
			i=-1;
			*j=h-qnt;
		}
	}
	
}


int main()
{
	int vet[TAM];
	for(int i=0; i<TAM; i++)
	{
		vet[i]=0;
	}
	int cont=0,x,y,z,c=0,j=0,atrib1,atrib2;
	do
	{ 
		printf("\nDigite:\n1 se deseja usar First fit\n2 se deseja Last (ou next) fit\n3 se deseja remover\n4 se deseja atribuir\n5 se deseja exibir\n9 se deseja sair\n");
		scanf("%d",&x);
		
		switch (x)
		{
			case 1:
				printf("Digite o ID do espaco a ser alocado\n");// numero que sera usado como identidade no vetor
				scanf("%d",&v[c].id);
				printf("Digite o TAMANHO do espaço a ser alocado\n");
				scanf("%d",&v[c].qnt);
				First_Fit(vet,v[c].qnt,v[c].id,&v[c].end);
				c++;
				break;
			
			case 2:
				printf("Digite o ID do espaco a ser alocado\n");
				scanf("%d",&v[c].id);
				printf("Digite o TAMANHO do espaço a ser alocado\n");
				scanf("%d",&v[c].qnt);
				Last_fit(vet, v[c].qnt, v[c].id, &v[c].end,c);
				printf("%d",v[c].end);
				c++;
				break;
				
			case 3: 
				printf("Digite o ID do espaco de memoria a ser deletado:\n");
				scanf("%d",&z);
				Delete ( vet, z);
				break;
				
			case 4:
				printf("Digite os valores que quer atribuir ordenadamente:\nValor 1: ");
				scanf("%d",&atrib1);
				printf("\nValor 2: ");
				scanf("%d",&atrib2);
				Atribuir(vet,atrib1,atrib2);		
				break;	
			
			case 5:
				Exibir(vet,c);
				break;	
				
			default:
				if(x==9)
				{
					return 0;
				}
				else
				printf("VALOR INDESEJADO");
				return 0;
				break;		
		}	
	}while(c<TAM || x==9);
	
	
	return 0;
}
