#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "dag.h"

static void imprimir_ordem(const char *rotulo, int *ordem, int tamanho) {
    if (ordem == NULL) {
        printf("%s: impossivel (grafo tem ciclo)\n", rotulo);
        return;
    }

    printf("%s: ", rotulo);
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", ordem[i]);
    }
    printf("\n");

    free(ordem);
}

int main(void) {
    printf("=== Grafo 1: DAG (dependencias de tarefas ao se vestir) ===\n");
    /* 0=meia, 1=sapato, 2=cueca, 3=calca, 4=camisa, 5=casaco */
    GrafoLista *g1 = criar_grafo_lista(6);
    inserir_aresta_lista(g1, 0, 1); /* meia -> sapato */
    inserir_aresta_lista(g1, 2, 3); /* cueca -> calca */
    inserir_aresta_lista(g1, 3, 1); /* calca -> sapato */
    inserir_aresta_lista(g1, 4, 5); /* camisa -> casaco */

    printf("Eh DAG? %s\n", eh_dag(g1) ? "sim" : "nao");

    int tamanho;
    int *ordem_kahn1 = ordenacao_topologica_kahn(g1, &tamanho);
    imprimir_ordem("Ordenacao (Kahn)", ordem_kahn1, tamanho);

    int *ordem_dfs1 = ordenacao_topologica_dfs(g1, &tamanho);
    imprimir_ordem("Ordenacao (DFS)", ordem_dfs1, tamanho);

    liberar_grafo_lista(g1);

    printf("\n=== Grafo 2: com ciclo (0 -> 1 -> 2 -> 0) ===\n");
    GrafoLista *g2 = criar_grafo_lista(3);
    inserir_aresta_lista(g2, 0, 1);
    inserir_aresta_lista(g2, 1, 2);
    inserir_aresta_lista(g2, 2, 0);

    printf("Eh DAG? %s\n", eh_dag(g2) ? "sim" : "nao");

    int *ordem_kahn2 = ordenacao_topologica_kahn(g2, &tamanho);
    imprimir_ordem("Ordenacao (Kahn)", ordem_kahn2, tamanho);

    int *ordem_dfs2 = ordenacao_topologica_dfs(g2, &tamanho);
    imprimir_ordem("Ordenacao (DFS)", ordem_dfs2, tamanho);

    liberar_grafo_lista(g2);

    return 0;
}
