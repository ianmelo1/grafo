#include <stddef.h>
#include "conectividade.h"

static int minimo(int a, int b) {
    return a < b ? a : b;
}

/* Algoritmo de Tarjan: descoberta[u] guarda a ordem em que u foi visitado
   e low[u] guarda o menor "descoberta" alcancavel a partir da subarvore
   de u, incluindo saltos por arestas de retorno. Um vertice u (que nao
   seja a raiz da DFS) e um vertice de articulacao se algum filho v dele
   satisfaz low[v] >= descoberta[u] (ou seja, a subarvore de v nao
   consegue "escapar" para cima sem passar por u). A raiz e articulacao
   se tiver mais de um filho na arvore de busca. */
void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo,
                       int *eh_articulacao) {
    visitado[u] = 1;
    descoberta[u] = low[u] = (*tempo)++;
    int filhos = 0;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;

        if (v == pai) {
            atual = atual->prox;
            continue;
        }

        if (visitado[v]) {
            low[u] = minimo(low[u], descoberta[v]);
        } else {
            filhos++;
            dfs_articulacoes(g, v, u, visitado, descoberta, low, tempo, eh_articulacao);
            low[u] = minimo(low[u], low[v]);

            if (pai != -1 && low[v] >= descoberta[u]) {
                eh_articulacao[u] = 1;
            }
        }

        atual = atual->prox;
    }

    if (pai == -1 && filhos > 1) {
        eh_articulacao[u] = 1;
    }
}

/* Uma aresta (u, v), com v filho de u na arvore de busca, e uma ponte
   quando low[v] > descoberta[u]: isso significa que a subarvore de v nao
   tem nenhuma outra ligacao (nem direta, nem por retorno) com u ou com
   qualquer ancestral de u, entao remover essa aresta desconectaria o
   grafo. As pontes encontradas sao guardadas em origens[]/destinos[]. */
void detectar_pontes(GrafoLista *g, int u, int pai, int *visitado,
                      int *descoberta, int *low, int *tempo,
                      int origens[], int destinos[], int *num_pontes) {
    visitado[u] = 1;
    descoberta[u] = low[u] = (*tempo)++;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;

        if (v == pai) {
            atual = atual->prox;
            continue;
        }

        if (visitado[v]) {
            low[u] = minimo(low[u], descoberta[v]);
        } else {
            detectar_pontes(g, v, u, visitado, descoberta, low, tempo, origens, destinos, num_pontes);
            low[u] = minimo(low[u], low[v]);

            if (low[v] > descoberta[u]) {
                origens[*num_pontes] = u;
                destinos[*num_pontes] = v;
                (*num_pontes)++;
            }
        }

        atual = atual->prox;
    }
}
