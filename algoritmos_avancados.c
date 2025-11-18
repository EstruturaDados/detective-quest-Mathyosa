#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Estrutura da sala da mansão

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;



//Árvore BST para pistas coletadas

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;



//Tabela Hash para pistas e suspeitos

#define TAM_HASH 17   // número primo para reduzir colisões

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];



//Função de Hash simples

unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str)
        h = (h * 31) + (*str++);
    return h % TAM_HASH;
}



//Insere uma pista e seu suspeito na tabela hash

void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int idx = hash(pista);

    HashNode *novo = malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}



//Encontra o suspeito baseado na pista

char* encontrarSuspeito(const char *pista) {
    unsigned int idx = hash(pista);
    HashNode *atual = tabelaHash[idx];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }

    return NULL;
}



//Cria uma nova sala

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista ? pista : "");
    nova->esq = nova->dir = NULL;
    return nova;
}



//Insere uma pista na árvore BST

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0)
        return raiz;

    if (raiz == NULL) {
        PistaNode *novo = malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}



//Mostra as pistas coletadas em ordem

void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}



//Contador para ver o suspeito final

int contadorSuspeito = 0;



//Conta quantas pistas apontam pro mesmo suspeito

void contarPistasDoSuspeito(PistaNode *raiz, const char *suspeito) {
    if (!raiz) return;

    contarPistasDoSuspeito(raiz->esq, suspeito);

    char *sus = encontrarSuspeito(raiz->pista);
    if (sus && strcmp(sus, suspeito) == 0)
        contadorSuspeito++;

    contarPistasDoSuspeito(raiz->dir, suspeito);
}


// Navegação pelas salas da mansão e coleta de pistas

void explorarSalas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf(">Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala\n");
        }

        printf("\nEscolha seu próximo passo:\n");
        if (atual->esq) printf(" (e) Ir para esquerda > %s\n", atual->esq->nome);
        if (atual->dir) printf(" (d) Ir para direita  > %s\n", atual->dir->nome);
        printf(" (s) Sair da exploração\n");
        printf("Opção: ");

        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esq) atual = atual->esq;
        else if (opcao == 'd' && atual->dir) atual = atual->dir;
        else if (opcao == 's') {
            printf("\nEncerrando exploração..\n");
            return;
        } else {
            printf("Opção inválida\n");
        }
    }
}



//Julgamento final do suspeito

void verificarSuspeitoFinal(PistaNode *arvorePistas) {
    char acusado[50];

    printf("\n= Julgamento Final =\n");
    printf("Digite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    contadorSuspeito = 0;
    contarPistasDoSuspeito(arvorePistas, acusado);

    printf("\nO suspeito \"%s\" possui %d pistas indicando sua culpa\n",
           acusado, contadorSuspeito);

    if (contadorSuspeito >= 2)
        printf("\n🟥 ACUSAÇÃO ACEITA!\nO suspeito provavelmente é o verdadeiro culpado\n");
    else
        printf("\n🟩 ACUSAÇÃO REJEITADA!\nNão há pistas suficientes contra ele\n");
}



//Main do programa

int main() {

    // Criando tabela hash (pista/suspeito)
 
    inserirNaHash("Pegadas suspeitas na poeira", "Sr Monteiro");
    inserirNaHash("Um livro derrubado no chão", "Clodoalda");
    inserirNaHash("Anotações rasgadas sobre venenos", "Doutor Frankstein");
    inserirNaHash("Uma partitura marcada com sangue", "Math");
    inserirNaHash("Terra revirada recentemente", "Jardineiro junior");

    //Montando a mansão 
 
    Sala *hall = criarSala("Hall de Entrada", "Pegadas suspeitas na poeira");

    hall->esq = criarSala("Sala de Estar", "Um livro derrubado no chão");
    hall->dir = criarSala("Corredor Principal", "");

    hall->esq->esq = criarSala("Biblioteca", "Anotações rasgadas sobre venenos");
    hall->esq->dir = criarSala("Cozinha", "");

    hall->dir->esq = criarSala("Sala de Música", "Uma partitura marcada com sangue");
    hall->dir->dir = criarSala("Jardim Interno", "Terra revirada recentemente");

    //Árvore de pistas coletadas

    PistaNode *arvorePistas = NULL;

    printf("= Detective Quest: Missão Final =\n");
    explorarSalas(hall, &arvorePistas);

    // Exibir pistas 

    printf("\n= Pistas Coletadas =\n");
    if (!arvorePistas)
        printf("Nenhuma pista coletada\n");
    else
        exibirPistas(arvorePistas);

   
    //Julgamento final

    verificarSuspeitoFinal(arvorePistas);

    return 0;
}

