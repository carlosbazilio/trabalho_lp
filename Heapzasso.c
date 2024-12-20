//Leonardo Amaral
//Guilherme Guimarães
//Pedro Lippi

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct lista {
    int inicio;
    int tamanho;
    struct lista* prox;
} lista;

lista* PontoBusca = NULL; // Ponteiro que aponta para onde a busca deve começar

// Inicializa a lista de blocos livres com um único bloco de tamanho N
void inicializar_lista(lista** L, int N) 
{
    *L = (lista*)malloc(sizeof(lista));
    (*L)->inicio = 0;
    (*L)->tamanho = N;
    (*L)->prox = NULL;
}

// Inicializa a heap com valores 0, indicando memória livre
void inicializa_heap(int** heap, int N) 
{
    *heap = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        (*heap)[i] = 0; // 0 representa que o espaço está vazio
    }
}

// Função para alocar um processo com o algoritmo First Fit
int first_fit(int* heap, lista** L, int N) 
{
    lista* aux = *L;
    while (aux != NULL) 
    {
        if (aux->tamanho >= N) 
        { // Foi encontrado um espaço que possa armazenar o processo
            int inicio = aux->inicio;
            for (int i = aux->inicio; i < aux->inicio + N; i++) 
            {
                heap[i] = 1;
            }

            // Ajustar área livre
            if (aux->tamanho == N) 
            { // Área livre é igual ao tamanho do processo
                *L = aux->prox;
                free(aux);
            } else { // Caso haja espaço livre restante
                aux->inicio = aux->inicio + N;
                aux->tamanho = aux->tamanho - N;
            }
            PontoBusca = aux->prox; // Atualiza o ponteiro de busca
            return inicio;
        }
        aux = aux->prox;
    }
    return -1; // Não há memória suficiente 
}

// Função para alocar um processo com o algoritmo Next Fit
int next_fit(int* heap, lista** L, int N) 
{
    lista* aux = PontoBusca ? PontoBusca : *L;
    while (aux != NULL) 
    {
        if (aux->tamanho >= N) 
        { // Foi encontrado um espaço que possa armazenar o processo
            int inicio = aux->inicio;
            for (int i = aux->inicio; i < aux->inicio + N; i++) 
            {
                heap[i] = 1;
            }

            if (aux->tamanho == N) 
            {
                if (aux == *L) 
                {
                    *L = aux->prox;
                } else 
                {
                    aux->prox = aux->prox->prox;
                }
                free(aux);
            } else 
            {
                aux->inicio = aux->inicio + N;
                aux->tamanho = aux->tamanho - N;
            }
            PontoBusca = aux->prox;
            return inicio;
        }
        aux = aux->prox;
    }
    return -1;
}

// Função para alocar um processo com o algoritmo Best Fit
int best_fit(int* heap, lista** L, int N) 
{
    int bloco_mais_adequado = -1;
    int min_diferenca = __INT_MAX__; // Inicializa com um valor muito alto
    lista* aux = *L;

    while (aux != NULL) 
    {
        if (aux->tamanho >= N) 
        { // Encontrou um espaço que suporta o processo
            int diferenca = aux->tamanho - N;
            if (diferenca < min_diferenca) 
            { // Encontrou o melhor bloco (menor diferença)
                min_diferenca = diferenca;
                bloco_mais_adequado = aux->inicio;
            }
        }
        aux = aux->prox;
    }

    if (bloco_mais_adequado != -1) 
    {
        aux = *L;
        while (aux != NULL) 
        {
            if (aux->inicio == bloco_mais_adequado) 
            {
                for (int i = aux->inicio; i < aux->inicio + N; i++) 
                {
                    heap[i] = 1;
                }

                if (aux->tamanho == N) 
                {
                    *L = aux->prox;
                    free(aux);
                } else 
                {
                    aux->inicio = aux->inicio + N;
                    aux->tamanho = aux->tamanho - N;
                }
                PontoBusca = aux->prox;
                return bloco_mais_adequado;
            }
            aux = aux->prox;
        }
    }
    return -1; // Não há memória suficiente
}

// Função para alocar um processo com o algoritmo Worst Fit
int worst_fit(int* heap, lista** L, int N) 
{
    int bloco_mais_adequado = -1;
    int max_diferenca = -1; // Inicializa com um valor muito baixo
    lista* aux = *L;

    while (aux != NULL) 
    {
        if (aux->tamanho >= N) 
        { // Encontrou um espaço que suporta o processo
            int diferenca = aux->tamanho - N;
            if (diferenca > max_diferenca) 
            { // Encontrou o maior bloco (maior diferença)
                max_diferenca = diferenca;
                bloco_mais_adequado = aux->inicio;
            }
        }
        aux = aux->prox;
    }

    if (bloco_mais_adequado != -1) 
    {
        aux = *L;
        while (aux != NULL) 
        {
            if (aux->inicio == bloco_mais_adequado) 
            {
                for (int i = aux->inicio; i < aux->inicio + N; i++) 
                {
                    heap[i] = 1;
                }

                if (aux->tamanho == N) 
                {
                    *L = aux->prox;
                    free(aux);
                } else 
                {
                    aux->inicio = aux->inicio + N;
                    aux->tamanho = aux->tamanho - N;
                }
                PontoBusca = aux->prox;
                return bloco_mais_adequado;
            }
            aux = aux->prox;
        }
    }
    return -1; // Não há memória suficiente
}

// Função para remover um processo da heap e liberar o espaço
void remover_processos(int* heap, lista** L, int inicio, int tamanho) 
{
    // Marca as posições na heap como livres (0)
    for (int i = inicio; i < inicio + tamanho; i++) 
    {
        heap[i] = 0;
    }

    // Agora precisamos adicionar esse espaço de volta à lista de blocos livres
    lista* novo_bloco = (lista*)malloc(sizeof(lista));
    novo_bloco->inicio = inicio;
    novo_bloco->tamanho = tamanho;
    novo_bloco->prox = NULL;

    // Insere o novo bloco na lista de blocos livres de forma ordenada
    if (*L == NULL) 
    {
        *L = novo_bloco;
    } else 
    {
        lista* aux = *L;
        lista* ant = NULL;

        // Encontrar o ponto de inserção na lista ordenada
        while (aux != NULL && aux->inicio < novo_bloco->inicio) 
        {
            ant = aux;
            aux = aux->prox;
        }

        // Se for o início da lista ou lista vazia, insere no início
        if (ant == NULL) 
        {
            novo_bloco->prox = *L;
            *L = novo_bloco;
        } else 
        {
            ant->prox = novo_bloco;
            novo_bloco->prox = aux;
        }
    }

    // Tenta combinar blocos adjacentes de memória livre 
    lista* aux = *L;
    while (aux != NULL && aux->prox != NULL) 
    {
        // Se o bloco atual e o próximo são adjacentes, combina-os
        if (aux->inicio + aux->tamanho == aux->prox->inicio) 
        {
            aux->tamanho += aux->prox->tamanho;
            lista* temp = aux->prox;
            aux->prox = aux->prox->prox;
            free(temp);
        } else 
        {
            aux = aux->prox;
        }
    }
}

int main() 
{
    int op = 0;
    int op2;
    int N;
    int tamanho;
    lista* l = NULL;
    int* heap = NULL;

    printf("Digite o tamanho da memoria que deseja simular:\n");
    scanf("%d", &N);

    inicializar_lista(&l, N);
    inicializa_heap(&heap, N);

    for (int j = 0; j < N; j++)
        printf("%d ", heap[j]);

    while (1) 
    {
        printf("\n1 - Alterar tamanho do vetor\n2 - Alocar na heap\n3 - Remover palavra\n4 - Imprimir heap\n0 - Encerrar\n-> ");
        scanf("%d", &op);

        if (op == 0) 
        {
            break;
        }

        if (op == 1) 
        {
            printf("\nEscolha o tamanho da alocacao de memoria\n-> ");
            scanf("%d", &N);
            inicializa_heap(&heap, N); // Re-aloca o heap
            inicializar_lista(&l, N);  // Re-inicializa a lista
        }

        if (op == 2) 
        {
            printf("\nDigite o tamanho do processo: ");
            scanf("%d", &tamanho);
            printf("\nDigite qual tipo de alocação deseja: ");
            printf("\n1 - First fit\n2 - Next fit\n3 - Best fit\n4 - Worst fit\n-> ");
            scanf("%d", &op2);

            if (op2 == 1) 
            {
                int inicio = first_fit(heap, &l, tamanho); // Passa a lista por referência
                if (inicio != -1) 
                {
                    printf("Processo alocado a partir da posição %d\n", inicio);
                } else 
                {
                    printf("Memória insuficiente para alocar o processo\n");
                }
            }
            if (op2 == 2) 
            {
                int inicio = next_fit(heap, &l, tamanho); // Passa a lista por referência
                if (inicio != -1) 
                {
                    printf("Processo alocado a partir da posição %d\n", inicio);
                } else 
                {
                    printf("Memória insuficiente para alocar o processo\n");
                }
            }
            if (op2 == 3) 
            {
                int inicio = best_fit(heap, &l, tamanho); // Passa a lista por referência
                if (inicio != -1) 
                {
                    printf("Processo alocado a partir da posição %d\n", inicio);
                } else 
                {
                    printf("Memória insuficiente para alocar o processo\n");
                }
            }
            if (op2 == 4) 
            {
                int inicio = worst_fit(heap, &l, tamanho); // Passa a lista por referência
                if (inicio != -1) 
                {
                    printf("Processo alocado a partir da posição %d\n", inicio);
                } else 
                {
                    printf("Memória insuficiente para alocar o processo\n");
                }
            }
        }

        if (op == 3) 
        {
            int inicio_remover, tamanho_remover;
            printf("\nDigite a posição de início do processo a ser removido: ");
            scanf("%d", &inicio_remover);
            printf("Digite o tamanho do processo a ser removido: ");
            scanf("%d", &tamanho_remover);

            // Chama a função de remoção
            remover_processos(heap, &l, inicio_remover, tamanho_remover);
            printf("Processo removido com sucesso.\n");
        }

        if (op == 4) 
        {
            printf("\nHeap atual: ");
            for (int i = 0; i < N; i++) 
            {
                printf("%d ", heap[i]);
            }
            printf("\n");
        }
    }

    free(heap);
    return 0;
}
