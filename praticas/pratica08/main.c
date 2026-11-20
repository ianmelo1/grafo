#include <stdio.h>
#include "grafo_lista_ponderado.h"
#include "agm.h"

#define NUM_VERTICES 5

void imprimir_agm(Aresta *agm, int num_arestas, int custo)
{
    for (int i = 0; i < num_arestas; i++)
    {
        printf("  (%c, %c, %i)\n", 'A' + agm[i].u, 'A' + agm[i].v, agm[i].peso);
    }
    printf("Custo da AGM: %i\n\n", custo);
}

int main()
{
    // V = {A, B, C, D, E}
    GrafoLista *grafo = criar_grafo(NUM_VERTICES);

    // E = {(A,B,2), (A,C,3), (B,C,1), (B,D,4), (C,D,5), (C,E,7), (D,E,6)}
    adicionar_aresta(grafo, 0, 1, 2);
    adicionar_aresta(grafo, 0, 2, 3);
    adicionar_aresta(grafo, 1, 2, 1);
    adicionar_aresta(grafo, 1, 3, 4);
    adicionar_aresta(grafo, 2, 3, 5);
    adicionar_aresta(grafo, 2, 4, 7);
    adicionar_aresta(grafo, 3, 4, 6);

    printf("Grafo nao orientado ponderado\n");
    imprimir_grafo(grafo);

    Aresta agm[NUM_VERTICES - 1];

    printf("Kruskal:\n");
    int custo = kruskal(grafo, agm);
    imprimir_agm(agm, NUM_VERTICES - 1, custo);

    printf("Prim (a partir de A):\n");
    custo = prim(grafo, 0, agm);
    imprimir_agm(agm, NUM_VERTICES - 1, custo);

    liberar_grafo(grafo);
    return 0;
}
