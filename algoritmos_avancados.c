#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura da árvore de salas da mansão

typedef struct Sala {
    char nome[50];          // Nome da sala
    char pista[100];        // Pista
    struct Sala *esq;       // Caminho a esquerda
    struct Sala *dir;       // Caminho a direita
} Sala;


// Estrutura da árvore 

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;


// Cria dinamicamente um local na mansão

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");

    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}


// Insere uma pista coletada na arvore

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0)
        return raiz;

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// Exibir pistas coletadas


void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

// Navegação entre salas e coleta automática de pistas

void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Coleta de pista
        if (strlen(atual->pista) > 0) {
            printf(">> Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("\nEscolha seu próximo passo:\n");

        if (atual->esq != NULL) printf(" (e) Ir para esquerda -> %s\n", atual->esq->nome);
        if (atual->dir != NULL) printf(" (d) Ir para direita  -> %s\n", atual->dir->nome);

        printf(" (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        }
        else if (opcao == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        }
        else if (opcao == 's') {
            printf("\nEncerrando exploração...\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}


// Monta a mansão e inicia exploração

int main() {


    // Montagem da mansão (salas)
  
    Sala *hall = criarSala("Hall de Entrada", "Pegadas suspeitas na poeira");

    hall->esq = criarSala("Sala de Estar", "Um livro derrubado no chão");
    hall->dir = criarSala("Corredor Principal", "");

    hall->esq->esq = criarSala("Biblioteca", "Anotações rasgadas sobre venenos");
    hall->esq->dir = criarSala("Cozinha", "");

    hall->dir->esq = criarSala("Sala de Música", "Uma partitura marcada com sangue");
    hall->dir->dir = criarSala("Jardim Interno", "Terra revirada recentemente");

    // Árvore para pistas coletadas
    PistaNode *arvorePistas = NULL;

    printf("=== Detective Quest: Coleta de Pistas ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibir pistas coletadas
    
    printf("\n=== Pistas Coletadas (ordenadas) ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    return 0;
}