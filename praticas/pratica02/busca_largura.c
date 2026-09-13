#include <stdlib.h>
#include "busca_largura.h"

Fila *criar_fila(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}

void enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) return;
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int desenfileirar(Fila *f) {
    if (fila_vazia(f)) return -1;
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void liberar_fila(Fila *f) {
    if (!f) return;
    free(f->dados);
    free(f);
}

/* BFS a partir de "origem": calcula distancia e predecessor de cada vertice */
void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila *f = criar_fila(g->n);
    dist[origem] = 0;
    enfileirar(f, origem);

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(f, v);
            }
            atual = atual->prox;
        }
    }

    liberar_fila(f);
}

/* Testa bipartição via 2-coloracao com BFS, cobrindo todos os componentes */
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

/* Conta componentes conexos varrendo o grafo com BFS a partir de cada vertice nao visitado */
int contar_componentes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int componentes = 0;

    for (int inicio = 0; inicio < g->n; inicio++) {
        if (visitado[inicio]) continue;

        componentes++;
        visitado[inicio] = 1;
        Fila *f = criar_fila(g->n);
        enfileirar(f, inicio);

        while (!fila_vazia(f)) {
            int u = desenfileirar(f);
            No *atual = g->adj[u];
            while (atual != NULL) {
                int v = atual->destino;
                if (!visitado[v]) {
                    visitado[v] = 1;
                    enfileirar(f, v);
                }
                atual = atual->prox;
            }
        }

        liberar_fila(f);
    }

    free(visitado);
    return componentes;
}
