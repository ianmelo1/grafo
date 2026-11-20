#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "coloracao.h"

static void testar(const char *nome, GrafoLista *g) {
    printf("--- %s (n=%d) ---\n", nome, g->n);

    int num_cores;
    int *cor_gulosa = coloracao_gulosa(g, &num_cores);
    printf("Guloso: %d cor(es) -> ", num_cores);
    for (int i = 0; i < g->n; i++) printf("%d ", cor_gulosa[i]);
    printf("\n");
    free(cor_gulosa);

    int *cor_wp = coloracao_welsh_powell(g, &num_cores);
    printf("Welsh-Powell: %d cor(es) -> ", num_cores);
    for (int i = 0; i < g->n; i++) printf("%d ", cor_wp[i]);
    printf("\n");
    free(cor_wp);

    printf("Eh bipartido? %s\n\n", eh_bipartido(g) ? "sim" : "nao");
}

int main(void) {
    GrafoLista *triangulo = criar_grafo_lista(3);
    inserir_aresta_lista(triangulo, 0, 1);
    inserir_aresta_lista(triangulo, 1, 2);
    inserir_aresta_lista(triangulo, 2, 0);
    testar("Triangulo (K3)", triangulo);
    liberar_grafo_lista(triangulo);

    GrafoLista *quadrado = criar_grafo_lista(4);
    inserir_aresta_lista(quadrado, 0, 1);
    inserir_aresta_lista(quadrado, 1, 2);
    inserir_aresta_lista(quadrado, 2, 3);
    inserir_aresta_lista(quadrado, 3, 0);
    testar("Quadrado (ciclo de 4 vertices)", quadrado);
    liberar_grafo_lista(quadrado);

    /* Grafo coroa (K3,3 menos um emparelhamento perfeito): bipartido, logo
       o numero cromatico real e 2, mas o guloso com ordem 0..5 usa 3
       cores por causa da ordem de visita. Mostra por que a ordem importa
       no algoritmo guloso -- e que Welsh-Powell e so uma heuristica, nao
       uma garantia: como aqui todos os vertices tem o mesmo grau (2), o
       criterio de "ordenar por grau" nao desempata e o resultado pode
       continuar o mesmo. */
    GrafoLista *coroa = criar_grafo_lista(6);
    inserir_aresta_lista(coroa, 0, 3);
    inserir_aresta_lista(coroa, 0, 5);
    inserir_aresta_lista(coroa, 1, 2);
    inserir_aresta_lista(coroa, 1, 4);
    inserir_aresta_lista(coroa, 2, 5);
    inserir_aresta_lista(coroa, 3, 4);
    testar("Grafo coroa (bipartido, sensivel a ordem)", coroa);
    liberar_grafo_lista(coroa);

    return 0;
}
