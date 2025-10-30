#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 100
#define MAX_STRUCTS 100
#define MAX_NOME 10

typedef struct {
    char nome[MAX_NOME];
    int inicio;
    int tamanho;
    int ocupado;
} Bloco;

Bloco memoria[MAX_STRUCTS];
int total_blocos = 0;
int politica = 0; // 0 = first, 1 = best, 2 = worst

void inicializarHeap();
void definirPolitica();
void criarBloco();
void deletarBloco();
void mostrarHeap();
void atribuir();
void juntarEspacosLivres();

int main() {
    int opcao;

    printf("=== Simulador de Heap ===\n");
    inicializarHeap();

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1 - Definir Politica de Alocacao\n");
        printf("2 - Criar Novo Bloco (new)\n");
        printf("3 - Exibir Estado do Heap (exibe)\n");
        printf("4 - Deletar Bloco (del)\n");
        printf("5 - Fazer Atribuicao (b = a)\n");
        printf("6 - Sair\n");
        printf("Opcao: ");

        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Digite um numero (1-6): ");
            while (getchar() != '\n');
        }

        switch (opcao) {
        case 1: definirPolitica(); break;
        case 2: criarBloco(); break;
        case 3: mostrarHeap(); break;
        case 4: deletarBloco(); break;
        case 5: atribuir(); break;
        case 6: printf("Encerrando o programa...\n"); break;
        default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}

void inicializarHeap() {
    strcpy(memoria[0].nome, "(livre)");
    memoria[0].inicio = 0;
    memoria[0].tamanho = MAX_HEAP_SIZE;
    memoria[0].ocupado = 0;
    total_blocos = 1;
}

void definirPolitica() {
    int escolha;
    printf("\n--- Definir Politica ---\n");
    printf("1 - First Fit\n");
    printf("2 - Best Fit\n");
    printf("3 - Worst Fit\n");
    printf("Opcao: ");

    while (scanf("%d", &escolha) != 1) {
        printf("Entrada invalida. Digite um numero (1-3): ");
        while (getchar() != '\n');
    }

    switch (escolha) {
    case 1: politica = 0; printf("Politica definida: First Fit\n"); break;
    case 2: politica = 1; printf("Politica definida: Best Fit\n"); break;
    case 3: politica = 2; printf("Politica definida: Worst Fit\n"); break;
    default: printf("Opcao invalida. Mantendo a politica atual.\n");
    }
}

void criarBloco() {
    char nome[MAX_NOME];
    int tamanho, indiceEscolhido = -1, espacoEscolhido = -1;

    if (politica == 1) espacoEscolhido = 100001;

    printf("\n--- Criar Bloco (new) ---\n");
    printf("Digite o nome do bloco: ");
    scanf("%s", nome);
    printf("Digite o tamanho do bloco (em bytes): ");
    while (scanf("%d", &tamanho) != 1) {
        printf("Entrada invalida. Digite um tamanho (numero): ");
        while (getchar() != '\n');
    }

    for (int i = 0; i < total_blocos; i++) {
        if (strcmp(memoria[i].nome, nome) == 0) {
            printf("ERRO: nome '%s' ja existe.\n", nome);
            return;
        }
    }

    for (int i = 0; i < total_blocos; i++) {
        if (!memoria[i].ocupado && memoria[i].tamanho >= tamanho) {
            if (politica == 0) { // first fit
                indiceEscolhido = i;
                break;
            }
            else if (politica == 1 && memoria[i].tamanho < espacoEscolhido) {
                indiceEscolhido = i;
                espacoEscolhido = memoria[i].tamanho;
            }
            else if (politica == 2 && memoria[i].tamanho > espacoEscolhido) {
                indiceEscolhido = i;
                espacoEscolhido = memoria[i].tamanho;
            }
        }
    }

    if (indiceEscolhido != -1) {
        Bloco* bloco_livre = &memoria[indiceEscolhido];
        int tamanho_restante = bloco_livre->tamanho - tamanho;

        if (tamanho_restante == 0) {
            bloco_livre->ocupado = 1;
            strcpy(bloco_livre->nome, nome);
        }
        else {
            if (total_blocos >= MAX_STRUCTS) {
                printf("ERRO: limite de fragmentos atingido!\n");
                return;
            }

            Bloco novo_bloco;
            strcpy(novo_bloco.nome, nome);
            novo_bloco.tamanho = tamanho;
            novo_bloco.inicio = bloco_livre->inicio;
            novo_bloco.ocupado = 1;

            bloco_livre->tamanho = tamanho_restante;
            bloco_livre->inicio += tamanho;
            strcpy(bloco_livre->nome, "(livre)");

            for (int j = total_blocos; j > indiceEscolhido; j--) {
                memoria[j] = memoria[j - 1];
            }

            memoria[indiceEscolhido] = novo_bloco;
            total_blocos++;
        }

        printf("Bloco %s criado (%d bytes) usando %s Fit.\n", nome, tamanho,
            politica == 0 ? "First" : (politica == 1 ? "Best" : "Worst"));
    }
    else {
        printf("ERRO: nao ha espaco livre suficiente para %s (%d bytes).\n", nome, tamanho);
    }
}

void juntarEspacosLivres() {
    for (int i = 0; i < total_blocos - 1; i++) {
        if (!memoria[i].ocupado && !memoria[i + 1].ocupado) {
            memoria[i].tamanho += memoria[i + 1].tamanho;
            for (int j = i + 1; j < total_blocos - 1; j++) {
                memoria[j] = memoria[j + 1];
            }
            total_blocos--;
            i--;
        }
    }
}

void mostrarHeap() {
    printf("\n--- Estado Atual da Memoria (Total de blocos: %d) ---\n", total_blocos);
    for (int i = 0; i < total_blocos; i++) {
        printf("  - Bloco [%d]: %s\n", i, memoria[i].ocupado ? memoria[i].nome : "(livre)");
        printf("    Status:  %s\n", memoria[i].ocupado ? "OCUPADO" : "LIVRE");
        printf("    Inicio:  %d\n", memoria[i].inicio);
        printf("    Tamanho: %d bytes\n", memoria[i].tamanho);
    }
    printf("------------------------------------------------------\n");
}

void deletarBloco() {
    char nome[MAX_NOME];
    printf("\n--- Deletar Bloco (del) ---\n");
    printf("Digite o nome do bloco a deletar: ");
    scanf("%s", nome);

    for (int i = 0; i < total_blocos; i++) {
        if (memoria[i].ocupado && strcmp(memoria[i].nome, nome) == 0) {
            memoria[i].ocupado = 0;
            strcpy(memoria[i].nome, "(livre)");
            printf("Bloco %s liberado (%d bytes).\n", nome, memoria[i].tamanho);
            juntarEspacosLivres();
            return;
        }
    }
    printf("Bloco %s nao encontrado ou ja esta livre!\n", nome);
}

void atribuir() {
    char var1[MAX_NOME], var2[MAX_NOME];
    printf("\n--- Atribuir (var1 = var2) ---\n");
    printf("Digite o nome da variavel destino (var1): ");
    scanf("%s", var1);
    printf("Digite o nome da variavel origem (var2): ");
    scanf("%s", var2);

    int indiceOrigem = -1;
    for (int i = 0; i < total_blocos; i++) {
        if (memoria[i].ocupado && strcmp(memoria[i].nome, var2) == 0) {
            indiceOrigem = i;
            break;
        }
    }

    if (indiceOrigem == -1) {
        printf("Variavel de origem %s nao encontrada!\n", var2);
        return;
    }

    for (int i = 0; i < total_blocos; i++) {
        if (strcmp(memoria[i].nome, var1) == 0) {
            memoria[i].inicio = memoria[indiceOrigem].inicio;
            memoria[i].tamanho = memoria[indiceOrigem].tamanho;
            memoria[i].ocupado = 1;
            printf("%s agora aponta para o mesmo espaco que %s.\n", var1, var2);
            return;
        }
    }

    printf("ERRO: variavel de destino %s nao existe. (crie com 'new' primeiro).\n", var1);
}



