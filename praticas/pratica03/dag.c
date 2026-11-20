#include <stdlib.h>
#include "dag.h"

/* Fila simples (array circular), usada pelo algoritmo de Kahn */
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

/* Algoritmo de Kahn: BFS usando grau de entrada.
   Devolve NULL (e *tamanho = 0) se o grafo tiver ciclo. */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int *grau_entrada = calloc(g->n, sizeof(int));

    for (int u = 0; u < g->n; u++) {
        No *atual = g->adj[u];
        while (atual != NULL) {
            grau_entrada[atual->destino]++;
            atual = atual->prox;
        }
    }

    Fila *f = criar_fila(g->n);
    for (int u = 0; u < g->n; u++) {
        if (grau_entrada[u] == 0) enfileirar(f, u);
    }

    int *resultado = malloc(g->n * sizeof(int));
    int contador = 0;

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        resultado[contador++] = u;

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) enfileirar(f, v);
            atual = atual->prox;
        }
    }

    liberar_fila(f);
    free(grau_entrada);

    if (contador < g->n) {
        /* sobrou vertice de fora: existe ciclo, ordenacao impossivel */
        free(resultado);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = g->n;
    return resultado;
}

/* Deteccao de ciclo em grafo direcionado via DFS com 3 cores:
   BRANCO = nao visitado, CINZA = em andamento (na recursao atual),
   PRETO = ja terminou. Uma aresta para um vertice CINZA fecha um ciclo. */
enum { BRANCO, CINZA, PRETO };

static int tem_ciclo_aux(GrafoLista *g, int u, int *cor) {
    cor[u] = CINZA;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (cor[v] == CINZA) return 1;
        if (cor[v] == BRANCO && tem_ciclo_aux(g, v, cor)) return 1;
        atual = atual->prox;
    }

    cor[u] = PRETO;
    return 0;
}

int eh_dag(GrafoLista *g) {
    int *cor = calloc(g->n, sizeof(int));
    int tem_ciclo = 0;

    for (int u = 0; u < g->n && !tem_ciclo; u++) {
        if (cor[u] == BRANCO) {
            tem_ciclo = tem_ciclo_aux(g, u, cor);
        }
    }

    free(cor);
    return !tem_ciclo;
}

static void dfs_topo(GrafoLista *g, int u, int *visitado, int *pilha, int *topo) {
    visitado[u] = 1;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) dfs_topo(g, v, visitado, pilha, topo);
        atual = atual->prox;
    }

    pilha[(*topo)++] = u;
}

/* Ordenacao topologica via DFS: cada vertice e empilhado quando termina
   de ser explorado, entao a pilha invertida ja sai na ordem correta.
   Devolve NULL (e *tamanho = 0) se o grafo nao for um DAG. */
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    if (!eh_dag(g)) {
        *tamanho = 0;
        return NULL;
    }

    int *visitado = calloc(g->n, sizeof(int));
    int *pilha = malloc(g->n * sizeof(int));
    int topo = 0;

    for (int u = 0; u < g->n; u++) {
        if (!visitado[u]) dfs_topo(g, u, visitado, pilha, &topo);
    }

    int *resultado = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        resultado[i] = pilha[g->n - 1 - i];
    }

    free(visitado);
    free(pilha);
    *tamanho = g->n;
    return resultado;
}
