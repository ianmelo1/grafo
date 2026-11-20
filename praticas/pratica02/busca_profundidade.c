#include <stdlib.h>
#include "busca_profundidade.h"

Pilha *criar_pilha(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    p->dados = malloc(capacidade * sizeof(int));
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

int pilha_vazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, int valor) {
    if (p->topo + 1 == p->capacidade) return;
    p->dados[++p->topo] = valor;
}

int desempilhar(Pilha *p) {
    if (pilha_vazia(p)) return -1;
    return p->dados[p->topo--];
}

void liberar_pilha(Pilha *p) {
    if (!p) return;
    free(p->dados);
    free(p);
}

/* DFS recursiva registrando tempo de entrada e saida de cada vertice */
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo, int *entrada, int *saida) {
    visitado[u] = 1;
    entrada[u] = (*tempo)++;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado, tempo, entrada, saida);
        }
        atual = atual->prox;
    }

    saida[u] = (*tempo)++;
}

/* Em um grafo nao-direcionado, ha ciclo se existir uma aresta para um
   vertice ja visitado que nao seja o pai na arvore de busca */
static int tem_ciclo_aux(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) {
            if (tem_ciclo_aux(g, v, u, visitado)) return 1;
        } else if (v != pai) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            if (tem_ciclo_aux(g, i, -1, visitado)) {
                free(visitado);
                return 1;
            }
        }
    }

    free(visitado);
    return 0;
}
