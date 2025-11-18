#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura para representar uma sala na mansão

typedef struct Sala {
    char nome[50];          // Nome da sala
    struct Sala *esq;       
} Sala;

// Função para criar uma nova sala

Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

//Explora as salas da mansão de forma interativa

void explorarSalas(Sala *atual) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);


        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Não há mais caminhos a seguir. A exploração terminou.\n");
            return;
        }

        printf("Escolha um caminho:\n");
        if (atual->esq != NULL) printf(" (e) Ir para a esquerda -> %s\n", atual->esq->nome);
        if (atual->dir != NULL) printf(" (d) Ir para a direita  -> %s\n", atual->dir->nome);
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
            printf("Você decidiu sair da mansão.\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Monta a mansão e inicia a exploração.

int main() {

    // Criando as salas da mansão
    Sala *hall        = criarSala("Hall de Entrada");

    // Nível 1
    hall->esq         = criarSala("Sala de Estar");
    hall->dir         = criarSala("Corredor Principal");

    // Nível 2 - Esquerda
    hall->esq->esq    = criarSala("Biblioteca");
    hall->esq->dir    = criarSala("Cozinha");

    // Nível 2 - Direita
    hall->dir->esq    = criarSala("Sala de Música");
    hall->dir->dir    = criarSala("Jardim Interno");

    // Início da exploração
    printf("Detective Quest: Exploração da Mansão \n");
    explorarSalas(hall);

    return 0;
}
