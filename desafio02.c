#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // opcional para medir tempo

/* ===========================================================
   Estruturas
   =========================================================== */

typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* Lista encadeada */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

/* Para a versão vetor */
#define MAX_ITENS 100

/* Contadores de comparações (para buscas) */
static long long compsSequencial = 0;
static long long compsBinaria    = 0;

/* ===========================================================
   Funções utilitárias
   =========================================================== */

void zeraContadores(void) {
    compsSequencial = 0;
    compsBinaria = 0;
}

/* Exibe um item */
void imprimeItem(const Item* it) {
    printf("Nome: %-28s | Tipo: %-18s | Qtd: %d\n",
           it->nome, it->tipo, it->quantidade);
}

/* Leitura segura de linha (removendo \n) */
void lerLinha(char *dest, size_t tam) {
    if (fgets(dest, (int)tam, stdin)) {
        size_t n = strlen(dest);
        if (n && dest[n-1] == '\n') dest[n-1] = '\0';
    }
}

/* ===========================================================
   MOCHILA COM VETOR (lista sequencial)
   =========================================================== */

/* Insere ao fim do vetor (sem ordenação) */
int inserirItemVetor(Item vet[], int *n) {
    if (*n >= MAX_ITENS) {
        puts("Mochila (vetor) cheia!");
        return 0;
    }
    Item novo;
    printf("Nome do item: ");
    lerLinha(novo.nome, sizeof(novo.nome));
    printf("Tipo do item: ");
    lerLinha(novo.tipo, sizeof(novo.tipo));
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar(); // consome '\n'

    vet[*n] = novo;
    (*n)++;
    puts("Item inserido no vetor.");
    return 1;
}

/* Remove por nome (primeira ocorrência) e desloca elementos */
int removerItemVetor(Item vet[], int *n, const char* nome) {
    for (int i = 0; i < *n; ++i) {
        if (strcmp(vet[i].nome, nome) == 0) {
            for (int j = i; j < *n - 1; ++j)
                vet[j] = vet[j+1];
            (*n)--;
            puts("Item removido do vetor.");
            return 1;
        }
    }
    puts("Item NAO encontrado no vetor.");
    return 0;
}

/* Listagem simples */
void listarVetor(const Item vet[], int n) {
    if (n == 0) { puts("Mochila (vetor) vazia."); return; }
    for (int i = 0; i < n; ++i) imprimeItem(&vet[i]);
}

/* Busca sequencial no vetor (conta comparações) */
int buscarSequencialVetor(const Item vet[], int n, const char* nome) {
    for (int i = 0; i < n; ++i) {
        compsSequencial++;              // comparação por item
        if (strcmp(vet[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Ordenação por nome (Selection Sort) */
void ordenarVetor(Item vet[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < n; ++j) {
            if (strcmp(vet[j].nome, vet[min].nome) < 0) {
                min = j;
            }
        }
        if (min != i) {
            Item tmp = vet[i];
            vet[i] = vet[min];
            vet[min] = tmp;
        }
    }
    puts("Vetor ordenado por nome (Selection Sort).");
}

/* Busca binária por nome num vetor já ordenado */
int buscarBinariaVetor(const Item vet[], int n, const char* nome) {
    int ini = 0, fim = n - 1;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        compsBinaria++;  // para contar a comparação “nome vs vet[meio].nome”
        int cmp = strcmp(nome, vet[meio].nome);
        if (cmp == 0) return meio;
        if (cmp < 0)   fim = meio - 1;
        else           ini = meio + 1;
    }
    return -1;
}

/* ===========================================================
   MOCHILA COM LISTA ENCADEADA
   =========================================================== */

No* criarNo(const Item* it) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) return NULL;
    novo->dados = *it;
    novo->proximo = NULL;
    return novo;
}

/* Insere no fim da lista */
int inserirItemLista(No** inicio) {
    Item novo;
    printf("Nome do item: ");
    lerLinha(novo.nome, sizeof(novo.nome));
    printf("Tipo do item: ");
    lerLinha(novo.tipo, sizeof(novo.tipo));
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar(); // consome '\n'

    No* no = criarNo(&novo);
    if (!no) { puts("Falha de alocacao na lista."); return 0; }

    if (*inicio == NULL) {
        *inicio = no;
    } else {
        No* aux = *inicio;
        while (aux->proximo) aux = aux->proximo;
        aux->proximo = no;
    }
    puts("Item inserido na lista.");
    return 1;
}

/* Remove por nome (primeira ocorrência) */
int removerItemLista(No** inicio, const char* nome) {
    No* ant = NULL;
    No* cur = *inicio;
    while (cur) {
        if (strcmp(cur->dados.nome, nome) == 0) {
            if (ant) ant->proximo = cur->proximo;
            else     *inicio      = cur->proximo;
            free(cur);
            puts("Item removido da lista.");
            return 1;
        }
        ant = cur;
        cur = cur->proximo;
    }
    puts("Item NAO encontrado na lista.");
    return 0;
}

/* Listagem da lista encadeada */
void listarLista(No* inicio) {
    if (!inicio) { puts("Mochila (lista) vazia."); return; }
    for (No* p = inicio; p; p = p->proximo) imprimeItem(&p->dados);
}

/* Busca sequencial na lista (conta comparações) */
No* buscarSequencialLista(No* inicio, const char* nome) {
    for (No* p = inicio; p; p = p->proximo) {
        compsSequencial++;  // cada nó comparado conta
        if (strcmp(p->dados.nome, nome) == 0) return p;
    }
    return NULL;
}

/* Libera toda a lista */
void liberarLista(No** inicio) {
    No* p = *inicio;
    while (p) {
        No* nxt = p->proximo;
        free(p);
        p = nxt;
    }
    *inicio = NULL;
}

/* ===========================================================
   Menus
   =========================================================== */

void menuVetor(void) {
    Item mochila[MAX_ITENS];
    int n = 0;
    int ordenado = 0; // flag: 0=nao; 1=sim

    for (;;) {
        printf("\n=== Mochila (VETOR) ===\n");
        printf("1. Inserir item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (sequencial)\n");
        printf("5. Ordenar por nome (Selection Sort)\n");
        printf("6. Buscar (binaria) [requer vetor ordenado]\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha: ");
        int op; if (scanf("%d", &op) != 1) return; getchar();

        if (op == 0) break;

        clock_t t0 = clock(); // medição opcional
        char nome[30];
        int idx;
        switch (op) {
            case 1:
                inserirItemVetor(mochila, &n);
                ordenado = 0; // inserir quebra ordenação
                break;

            case 2:
                printf("Nome do item a remover: ");
                lerLinha(nome, sizeof(nome));
                removerItemVetor(mochila, &n, nome);
                break;

            case 3:
                listarVetor(mochila, n);
                break;

            case 4: {
                printf("Nome a buscar (sequencial): ");
                lerLinha(nome, sizeof(nome));
                zeraContadores();
                idx = buscarSequencialVetor(mochila, n, nome);
                if (idx >= 0) {
                    printf(">> Encontrado no indice %d (vetor).\n", idx);
                    imprimeItem(&mochila[idx]);
                } else {
                    puts(">> Nao encontrado.");
                }
                printf("Comparacoes (sequencial): %lld\n", compsSequencial);
                break;
            }

            case 5:
                ordenarVetor(mochila, n);
                ordenado = 1;
                break;

            case 6: {
                if (!ordenado) {
                    puts("Vetor NAO esta ordenado. Ordene antes (opcao 5).");
                    break;
                }
                printf("Nome a buscar (binaria): ");
                lerLinha(nome, sizeof(nome));
                zeraContadores();
                idx = buscarBinariaVetor(mochila, n, nome);
                if (idx >= 0) {
                    printf(">> Encontrado no indice %d (vetor, binaria).\n", idx);
                    imprimeItem(&mochila[idx]);
                } else {
                    puts(">> Nao encontrado.");
                }
                printf("Comparacoes (binaria): %lld\n", compsBinaria);
                break;
            }

            default: puts("Opcao invalida.");
        }
        clock_t t1 = clock();
        double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("[Tempo da operacao: %.6f s]\n", tempo);
    }
}

void menuLista(void) {
    No* inicio = NULL;

    for (;;) {
        printf("\n=== Mochila (LISTA ENCADEADA) ===\n");
        printf("1. Inserir item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (sequencial)\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha: ");
        int op; if (scanf("%d", &op) != 1) return; getchar();

        if (op == 0) break;

        clock_t t0 = clock();
        char nome[30];
        No* achou;
        switch (op) {
            case 1:
                inserirItemLista(&inicio);
                break;

            case 2:
                printf("Nome do item a remover: ");
                lerLinha(nome, sizeof(nome));
                removerItemLista(&inicio, nome);
                break;

            case 3:
                listarLista(inicio);
                break;

            case 4:
                printf("Nome a buscar (sequencial, lista): ");
                lerLinha(nome, sizeof(nome));
                zeraContadores();
                achou = buscarSequencialLista(inicio, nome);
                if (achou) {
                    printf(">> Encontrado (lista).\n");
                    imprimeItem(&achou->dados);
                } else {
                    puts(">> Nao encontrado.");
                }
                printf("Comparacoes (sequencial - lista): %lld\n", compsSequencial);
                break;

            default: puts("Opcao invalida.");
        }
        clock_t t1 = clock();
        double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("[Tempo da operacao: %.6f s]\n", tempo);
    }

    liberarLista(&inicio);
}

/* ===========================================================
   Principal
   =========================================================== */

int main(void) {
    for (;;) {
        printf("\n==================== Menu Principal ====================\n");
        printf("1. Mochila com Vetor (lista sequencial)\n");
        printf("2. Mochila com Lista Encadeada\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        int op; if (scanf("%d", &op) != 1) return 0; getchar();

        if (op == 0) {
            puts("Encerrando...");
            break;
        }
        switch (op) {
            case 1: menuVetor(); break;
            case 2: menuLista(); break;
            default: puts("Opcao invalida.");
        }
    }
    return 0;
}
