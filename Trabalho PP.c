#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define tam_heap 12
bool heap[tam_heap] = {false};

typedef struct Node {
    int id;
    int end_inicial;
    int blocos_livres;
    struct Node* prox;
} Node;

Node* lista_areas_livres = NULL;
Node* lista_alocacoes = NULL;
int contador_id = 1;

void ImprimeHeap() {
    for (int i = 0; i < tam_heap; i++)
        printf("%d ", heap[i]);
    puts(""); // estética
}

void Inicializa_Lista() {
    lista_areas_livres = (Node*)malloc(sizeof(Node));
    lista_areas_livres->end_inicial = 0;
    lista_areas_livres->blocos_livres = tam_heap; // tam_heap == 12
    lista_areas_livres->prox = NULL;
}

void first(int valor, int* id_alocacao) {
    Node* No_atual = lista_areas_livres;
    Node* No_anterior = NULL;

    while (No_atual != NULL) {
        if (No_atual->blocos_livres >= valor) {
            *id_alocacao = contador_id++;
            for (int i = 0; i < valor; i++)
                heap[No_atual->end_inicial + i] = true;

            int inicio_alocacao = No_atual->end_inicial;

            if (No_atual->blocos_livres == valor) {
                if (No_anterior == NULL)
                    lista_areas_livres = No_atual->prox;
                else
                    No_anterior->prox = No_atual->prox;
                free(No_atual);
            } else {
                No_atual->blocos_livres -= valor;
                No_atual->end_inicial += valor;
            }

            Node* nova_alocacao = (Node*)malloc(sizeof(Node));
            nova_alocacao->id = *id_alocacao;
            nova_alocacao->end_inicial = inicio_alocacao;
            nova_alocacao->blocos_livres = valor;
            nova_alocacao->prox = lista_alocacoes;
            lista_alocacoes = nova_alocacao;

            break;
        }
        No_anterior = No_atual;
        No_atual = No_atual->prox;
    }
}

void next(int valor, int* id_alocacao) {
    static Node* ultima_alocacao = NULL;  // Mantém a referência da última alocação
    Node* No_atual = ultima_alocacao ? ultima_alocacao : lista_areas_livres;  // Começa da última alocação, ou do início se for a primeira alocação
    bool encontrou = false;

    while (No_atual != NULL) {
        if (No_atual->blocos_livres >= valor) {  // Verifica se o bloco tem espaço suficiente
            *id_alocacao = contador_id++;
            for (int i = 0; i < valor; i++)
                heap[No_atual->end_inicial + i] = true;  // Aloca os blocos

            int inicio_alocacao = No_atual->end_inicial;

            // Atualiza o estado após a alocação
            if (No_atual->blocos_livres == valor) {
                // Remove o nó atual da lista de áreas livres
                if (lista_areas_livres == No_atual) {
                    lista_areas_livres = No_atual->prox;
                } else {
                    Node* anterior = lista_areas_livres;
                    while (anterior->prox != No_atual)
                        anterior = anterior->prox;
                    anterior->prox = No_atual->prox;
                }
                free(No_atual);

                // Após remover o nó, `ultima_alocacao` deve ser atualizado para o nó seguinte
                ultima_alocacao = lista_areas_livres;  // Recomeça da lista principal
            } else {
                No_atual->end_inicial += valor;
                No_atual->blocos_livres -= valor;
                ultima_alocacao = No_atual;  // Atualiza a última alocação corretamente
            }

            // Adiciona a alocação à lista de alocações
            Node* nova_alocacao = (Node*)malloc(sizeof(Node));
            nova_alocacao->id = *id_alocacao;
            nova_alocacao->end_inicial = inicio_alocacao;
            nova_alocacao->blocos_livres = valor;
            nova_alocacao->prox = lista_alocacoes;
            lista_alocacoes = nova_alocacao;

            encontrou = true;
            break;
        }
        No_atual = No_atual->prox;  // Move para o próximo nó
    }

    // Caso não tenha encontrado espaço suficiente, começa a busca novamente do início
    if (!encontrou) {
        ultima_alocacao = NULL;  // Reinicia a busca a partir do início
        next(valor, id_alocacao);  // Tenta alocar novamente, começando do início
    }
}

void liberar(int id_alocacao) {
    Node* atual = lista_alocacoes;
    Node* anterior = NULL;
    bool encontrou = false;

    while (atual != NULL) {
        if (atual->id == id_alocacao) {
            encontrou = true;
            for (int i = 0; i < atual->blocos_livres; i++)
                heap[atual->end_inicial + i] = false;

            if (anterior == NULL)
                lista_alocacoes = atual->prox;
            else
                anterior->prox = atual->prox;

            Node* novo_no = (Node*)malloc(sizeof(Node));
            novo_no->end_inicial = atual->end_inicial;
            novo_no->blocos_livres = atual->blocos_livres;
            novo_no->prox = NULL;

            // Inserindo na lista de áreas livres, mantendo a lista ordenada por end_inicial
            if (lista_areas_livres == NULL || lista_areas_livres->end_inicial > novo_no->end_inicial) {
                novo_no->prox = lista_areas_livres;
                lista_areas_livres = novo_no;
            } else {
                Node* current = lista_areas_livres;
                while (current->prox != NULL && current->prox->end_inicial < novo_no->end_inicial)
                    current = current->prox;
                novo_no->prox = current->prox;
                current->prox = novo_no;
            }

            // Unindo blocos livres adjacentes, se houver
            Node* current = lista_areas_livres;
            Node* prev = NULL;
            while (current != NULL) {
                if (prev != NULL && prev->end_inicial + prev->blocos_livres == current->end_inicial) {
                    prev->blocos_livres += current->blocos_livres;
                    prev->prox = current->prox;
                    free(current);
                    current = prev->prox;
                } else {
                    prev = current;
                    current = current->prox;
                }
            }

            free(atual);
            ImprimeHeap();
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    if (!encontrou)
        printf("Erro: Não foi possível liberar o bloco com ID %d.\n", id_alocacao);
}

void LiberaLista(Node* lista) {
    Node* atual = lista;
    while (atual != NULL) {
        Node* temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

void LiberaRecursos() {
    LiberaLista(lista_areas_livres);
    LiberaLista(lista_alocacoes);
}

int main() {
    ImprimeHeap();
    Inicializa_Lista();

    while(1){
        char estrategia[5];
        printf("\nMODO(first, next, liberar): ");
        scanf("%s", estrategia);

        if(strcmp(estrategia, "first") == 0){
            char nome[10];
            int valor = 0;
            printf("Nome e valor: ");
            scanf("%s %d", nome, &valor);

            int id_alocacao = 0;
            first(valor, &id_alocacao);
            printf("Alocacao ID: %d\n", id_alocacao);
            ImprimeHeap();
        }
        else if(strcmp(estrategia, "next") == 0){
            char nome[10];
            int valor = 0;
            printf("Nome e valor: ");
            scanf("%s %d", nome, &valor);

            int id_alocacao = 0;
            next(valor, &id_alocacao);
            printf("Alocacao ID: %d\n", id_alocacao);
            ImprimeHeap();
        }
        else if (strcmp(estrategia, "liberar") == 0) {
            int id_alocacao = 0;
            printf("ID da alocacao a ser liberada: ");
            scanf("%d", &id_alocacao);

            liberar(id_alocacao);
            ImprimeHeap();
        }
        else
            printf("Opcao invalida\n");
    }

    LiberaRecursos();
}
