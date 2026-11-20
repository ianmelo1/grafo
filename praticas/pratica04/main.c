#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "conectividade.h"
#include "planaridade.h"

static void testar_conectividade(void) {
    printf("=== Conectividade: dois triangulos ligados por uma ponte ===\n");
    /* triangulo 0-1-2, triangulo 3-4-5, ponte 2-3 */
    GrafoLista *g = criar_grafo_lista(6);
    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 1, 2);
    inserir_aresta_lista(g, 2, 0);
    inserir_aresta_lista(g, 3, 4);
    inserir_aresta_lista(g, 4, 5);
    inserir_aresta_lista(g, 5, 3);
    inserir_aresta_lista(g, 2, 3);

    int *visitado = calloc(g->n, sizeof(int));
    int *descoberta = malloc(g->n * sizeof(int));
    int *low = malloc(g->n * sizeof(int));
    int *eh_articulacao = calloc(g->n, sizeof(int));
    int tempo = 0;

    for (int u = 0; u < g->n; u++) {
        if (!visitado[u]) {
            dfs_articulacoes(g, u, -1, visitado, descoberta, low, &tempo, eh_articulacao);
        }
    }

    printf("Vertices de articulacao: ");
    for (int u = 0; u < g->n; u++) {
        if (eh_articulacao[u]) printf("%d ", u);
    }
    printf("\n");

    free(visitado);
    free(descoberta);
    free(low);
    free(eh_articulacao);

    int *visitado2 = calloc(g->n, sizeof(int));
    int *descoberta2 = malloc(g->n * sizeof(int));
    int *low2 = malloc(g->n * sizeof(int));
    int *origens = malloc(g->n * sizeof(int));
    int *destinos = malloc(g->n * sizeof(int));
    int num_pontes = 0;
    int tempo2 = 0;

    for (int u = 0; u < g->n; u++) {
        if (!visitado2[u]) {
            detectar_pontes(g, u, -1, visitado2, descoberta2, low2, &tempo2, origens, destinos, &num_pontes);
        }
    }

    printf("Pontes encontradas: ");
    for (int i = 0; i < num_pontes; i++) {
        printf("(%d-%d) ", origens[i], destinos[i]);
    }
    printf("\n");

    free(visitado2);
    free(descoberta2);
    free(low2);
    free(origens);
    free(destinos);
    liberar_grafo_lista(g);
}

static void testar_planaridade(const char *nome, GrafoLista *g) {
    printf("--- %s (n=%d) ---\n", nome, g->n);
    printf("Satisfaz Euler (m <= 3n-6)? %s\n", eh_planar_euler(g) ? "sim" : "nao");
    printf("Contem K5? %s\n", contem_k5(g) ? "sim" : "nao");
    printf("Contem K3,3? %s\n", contem_k33(g) ? "sim" : "nao");
    printf("Eh planar (heuristica)? %s\n\n", eh_planar(g) ? "sim" : "nao");
}

int main(void) {
    testar_conectividade();

    printf("\n=== Planaridade ===\n");

    GrafoLista *quadrado = criar_grafo_lista(4);
    inserir_aresta_lista(quadrado, 0, 1);
    inserir_aresta_lista(quadrado, 1, 2);
    inserir_aresta_lista(quadrado, 2, 3);
    inserir_aresta_lista(quadrado, 3, 0);
    testar_planaridade("Quadrado (ciclo de 4 vertices)", quadrado);
    liberar_grafo_lista(quadrado);

    GrafoLista *k5 = criar_grafo_lista(5);
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            inserir_aresta_lista(k5, i, j);
        }
    }
    testar_planaridade("K5", k5);
    liberar_grafo_lista(k5);

    GrafoLista *k33 = criar_grafo_lista(6);
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) {
            inserir_aresta_lista(k33, i, j);
        }
    }
    testar_planaridade("K3,3", k33);
    liberar_grafo_lista(k33);

    return 0;
}
