#include <stdio.h>
#include <limits.h>
#include "grafo_lista_ponderado.h"
#include "dijkstra.h"

#define NUM_VERTICES 5

int main()
{
    // Grafo orientado da aula 04 (grafo_dijkstra.dot) + E sem arcos de chegada
    GrafoListaPonderado *grafo = criar_grafo(NUM_VERTICES);

    adicionar_arco(grafo, 0, 1, 4); // A -> B
    adicionar_arco(grafo, 0, 2, 1); // A -> C
    adicionar_arco(grafo, 1, 3, 1); // B -> D
    adicionar_arco(grafo, 2, 1, 2); // C -> B
    adicionar_arco(grafo, 2, 3, 5); // C -> D
    adicionar_arco(grafo, 4, 0, 3); // E -> A

    printf("Grafo orientado ponderado\n");
    imprimir_grafo(grafo);

    int dist[NUM_VERTICES];
    int pred[NUM_VERTICES];
    int origem = 0;

    dijkstra(grafo, origem, dist, pred);

    printf("Dijkstra a partir de %c:\n", 'A' + origem);
    for (int v = 0; v < NUM_VERTICES; v++)
    {
        printf("  %c: ", 'A' + v);
        if (dist[v] == INT_MAX)
        {
            printf("inalcancavel\n");
            continue;
        }
        printf("distancia %i, caminho ", dist[v]);
        reconstruir_caminho(origem, v, pred);
        printf("\n");
    }

    liberar_grafo(grafo);
    return 0;
}
