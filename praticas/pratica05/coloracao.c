#include <stdlib.h>
#include "coloracao.h"

/* Colore u com a menor cor que nenhum vizinho ja colorido esteja usando.
   "usada" e um array auxiliar de tamanho n (uma posicao por cor possivel,
   no maximo n cores podem ser necessarias). */
static int menor_cor_livre(GrafoLista *g, int u, int *cor) {
    int *usada = calloc(g->n, sizeof(int));

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (cor[v] != -1) usada[cor[v]] = 1;
        atual = atual->prox;
    }

    int c = 0;
    while (usada[c]) c++;

    free(usada);
    return c;
}

/* Algoritmo guloso basico: percorre os vertices na ordem 0, 1, 2, ...
   e da a cada um a menor cor ainda livre entre seus vizinhos. O
   resultado depende da ordem escolhida e nao garante o numero minimo
   de cores (numero cromatico). */
int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->n;
    int *cor = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) cor[i] = -1;

    for (int u = 0; u < n; u++) {
        cor[u] = menor_cor_livre(g, u, cor);
    }

    int maior = -1;
    for (int i = 0; i < n; i++) {
        if (cor[i] > maior) maior = cor[i];
    }
    *num_cores = maior + 1;

    return cor;
}

/* Heuristica Welsh-Powell: mesma ideia do guloso, mas os vertices sao
   coloridos em ordem decrescente de grau primeiro. Colorir os vertices
   mais conectados antes costuma reduzir o numero de cores usadas, mas
   ainda e uma heuristica (nao garante o numero cromatico minimo). */
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->n;

    int *ordem = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) ordem[i] = i;

    for (int i = 0; i < n - 1; i++) {
        int maior = i;
        for (int j = i + 1; j < n; j++) {
            if (grau_lista(g, ordem[j]) > grau_lista(g, ordem[maior])) {
                maior = j;
            }
        }
        int tmp = ordem[i];
        ordem[i] = ordem[maior];
        ordem[maior] = tmp;
    }

    int *cor = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) cor[i] = -1;

    for (int i = 0; i < n; i++) {
        int u = ordem[i];
        cor[u] = menor_cor_livre(g, u, cor);
    }

    free(ordem);

    int maior_cor = -1;
    for (int i = 0; i < n; i++) {
        if (cor[i] > maior_cor) maior_cor = cor[i];
    }
    *num_cores = maior_cor + 1;

    return cor;
}

/* Fila simples (array circular), usada pelo teste de bipartição via BFS */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

static Fila *criar_fila(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

static int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}

static void enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) return;
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

static int desenfileirar(Fila *f) {
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

static void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

/* Bipartido = coloravel com exatamente 2 cores. Testa via BFS
   pintando cada vertice com a cor oposta a de quem o descobriu; se
   algum vizinho ja tiver a mesma cor, o grafo nao e bipartido. */
int eh_bipartido(GrafoLista *g) {
    int *cor = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) cor[i] = -1;

    int bipartido = 1;

    for (int inicio = 0; inicio < g->n && bipartido; inicio++) {
        if (cor[inicio] != -1) continue;

        cor[inicio] = 0;
        Fila *f = criar_fila(g->n);
        enfileirar(f, inicio);

        while (!fila_vazia(f) && bipartido) {
            int u = desenfileirar(f);
            No *atual = g->adj[u];
            while (atual != NULL && bipartido) {
                int v = atual->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    enfileirar(f, v);
                } else if (cor[v] == cor[u]) {
                    bipartido = 0;
                }
                atual = atual->prox;
            }
        }

        liberar_fila(f);
    }

    free(cor);
    return bipartido;
}
