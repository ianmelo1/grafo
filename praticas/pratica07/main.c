#include <stdio.h>
#include <limits.h>
#include "grafo_lista_ponderado.h"
#include "bellman_ford.h"

static void imprimir_caminho(int origem, int destino, int *pred)
{
    if (destino != origem)
    {
        imprimir_caminho(origem, pred[destino], pred);
        printf(" -> ");
    }
    printf("%c", 'A' + destino);
}

static void executar(GrafoListaPonderado *g, int origem)
{
    int dist[MAX_VERTICES];
    int pred[MAX_VERTICES];

    imprimir_grafo(g);

    if (!bellman_ford(g, origem, dist, pred))
    {
        reconstruir_ciclo_negativo(g, dist, pred);
        printf("\n");
        return;
    }

    printf("Bellman-Ford a partir de %c:\n", 'A' + origem);
    for (int v = 0; v < g->num_vertices; v++)
    {
        printf("  %c: ", 'A' + v);
        if (dist[v] == INT_MAX)
        {
            printf("inalcancavel\n");
            continue;
        }
        printf("distancia %i, caminho ", dist[v]);
        imprimir_caminho(origem, v, pred);
        printf("\n");
    }
    printf("\n");
}

int main()
{
    // Pesos negativos sem ciclo negativo (aqui o Dijkstra erraria o B)
    GrafoListaPonderado *grafo = criar_grafo(5);
    adicionar_arco(grafo, 0, 1, 4);  // A -> B
    adicionar_arco(grafo, 0, 2, 5);  // A -> C
    adicionar_arco(grafo, 1, 3, 3);  // B -> D
    adicionar_arco(grafo, 2, 1, -3); // C -> B
    adicionar_arco(grafo, 3, 4, 2);  // D -> E

    printf("Grafo 1: pesos negativos, sem ciclo negativo\n");
    executar(grafo, 0);
    liberar_grafo(grafo);

    // Ciclo negativo B -> C -> D -> B: soma -1 - 1 - 1 = -3
    GrafoListaPonderado *grafo_ciclo = criar_grafo(4);
    adicionar_arco(grafo_ciclo, 0, 1, 1);  // A -> B
    adicionar_arco(grafo_ciclo, 1, 2, -1); // B -> C
    adicionar_arco(grafo_ciclo, 2, 3, -1); // C -> D
    adicionar_arco(grafo_ciclo, 3, 1, -1); // D -> B

    printf("Grafo 2: com ciclo negativo\n");
    executar(grafo_ciclo, 0);
    liberar_grafo(grafo_ciclo);

    return 0;
}
