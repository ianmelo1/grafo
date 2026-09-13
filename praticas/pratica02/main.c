#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

static void imprimir_bfs(GrafoLista *g, int origem) {
    int *dist = malloc(g->n * sizeof(int));
    int *pred = malloc(g->n * sizeof(int));

    bfs(g, origem, dist, pred);

    printf("BFS a partir do vertice %d:\n", origem);
    for (int i = 0; i < g->n; i++) {
        printf("  vertice %d -> dist = %d, pred = %d\n", i, dist[i], pred[i]);
    }

    free(dist);
    free(pred);
}

static void imprimir_dfs(GrafoLista *g, int origem) {
    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = malloc(g->n * sizeof(int));
    int *saida = malloc(g->n * sizeof(int));
    int tempo = 0;

    dfs_recursiva(g, origem, visitado, &tempo, entrada, saida);

    printf("DFS a partir do vertice %d:\n", origem);
    for (int i = 0; i < g->n; i++) {
        if (visitado[i]) {
            printf("  vertice %d -> entrada = %d, saida = %d\n", i, entrada[i], saida[i]);
        }
    }

    free(visitado);
    free(entrada);
    free(saida);
}

int main(void) {
    printf("=== Grafo 1: arvore conexa e bipartida ===\n");
    GrafoLista *g1 = criar_grafo_lista(6);
    inserir_aresta_lista(g1, 0, 1);
    inserir_aresta_lista(g1, 0, 2);
    inserir_aresta_lista(g1, 1, 3);
    inserir_aresta_lista(g1, 1, 4);
    inserir_aresta_lista(g1, 2, 5);

    imprimir_bfs(g1, 0);
    imprimir_dfs(g1, 0);
    printf("Numero de componentes: %d\n", contar_componentes(g1));
    printf("Possui ciclo? %s\n", tem_ciclo(g1) ? "sim" : "nao");
    printf("Eh bipartido? %s\n", eh_bipartido(g1) ? "sim" : "nao");

    liberar_grafo_lista(g1);

    printf("\n=== Grafo 2: com ciclo e vertices desconexos ===\n");
    GrafoLista *g2 = criar_grafo_lista(7);
    inserir_aresta_lista(g2, 0, 1);
    inserir_aresta_lista(g2, 1, 2);
    inserir_aresta_lista(g2, 2, 0);
    inserir_aresta_lista(g2, 3, 4);
    /* vertices 5 e 6 ficam isolados, formando componentes proprios */

    printf("Numero de componentes: %d\n", contar_componentes(g2));
    printf("Possui ciclo? %s\n", tem_ciclo(g2) ? "sim" : "nao");
    printf("Eh bipartido? %s\n", eh_bipartido(g2) ? "sim" : "nao");

    liberar_grafo_lista(g2);

    return 0;
}
