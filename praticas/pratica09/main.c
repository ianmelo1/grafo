#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

static void imprimir_cobertura(int *cobertura, int n, int tamanho)
{
    printf("{");
    int primeiro = 1;
    for (int v = 0; v < n; v++)
    {
        if (cobertura[v])
        {
            printf(primeiro ? "%c" : ", %c", 'A' + v);
            primeiro = 0;
        }
    }
    printf("} tamanho %i\n", tamanho);
}

// Cada par de vertices vira aresta com 50% de chance
static void gerar_grafo_aleatorio(Grafo *g, int n)
{
    inicializar_grafo(g, n);
    for (int u = 0; u < n; u++)
    {
        for (int v = u + 1; v < n; v++)
        {
            if (rand() % 2)
            {
                adicionar_aresta(g, u, v);
            }
        }
    }
}

int main()
{
    Grafo g;
    int cobertura[MAX_VERTICES];
    long chamadas;
    int tamanho;

    // Exemplo: caminho E - B - A - C - D
    // A gulosa pega A (grau 2, o primeiro empatado) e depois precisa de B e C
    inicializar_grafo(&g, 5);
    adicionar_aresta(&g, 0, 1); // A - B
    adicionar_aresta(&g, 0, 2); // A - C
    adicionar_aresta(&g, 1, 4); // B - E
    adicionar_aresta(&g, 2, 3); // C - D

    printf("Exemplo: caminho E - B - A - C - D\n");

    tamanho = cobertura_minima_bt(&g, cobertura, &chamadas);
    printf("  Backtracking: ");
    imprimir_cobertura(cobertura, g.num_vertices, tamanho);
    printf("    eh cobertura? %s\n", eh_cobertura(&g, cobertura) ? "sim" : "nao");

    tamanho = cobertura_gulosa(&g, cobertura);
    printf("  Gulosa:       ");
    imprimir_cobertura(cobertura, g.num_vertices, tamanho);
    printf("    eh cobertura? %s\n\n", eh_cobertura(&g, cobertura) ? "sim" : "nao");

    // Experimento: forca bruta x heuristica em grafos aleatorios
    int tamanhos[] = {5, 8, 10, 12, 15};
    srand(42); // semente fixa: sempre os mesmos grafos

    printf("Experimento (grafos aleatorios, 50%% de chance por aresta)\n");
    printf("   n | arestas | BT tam | BT nos visitados | BT tempo (ms) | Gul tam | Gul tempo (ms)\n");
    printf("-----+---------+--------+------------------+---------------+---------+---------------\n");

    for (int i = 0; i < 5; i++)
    {
        int n = tamanhos[i];
        gerar_grafo_aleatorio(&g, n);

        clock_t inicio = clock();
        int tam_bt = cobertura_minima_bt(&g, cobertura, &chamadas);
        double tempo_bt = 1000.0 * (clock() - inicio) / CLOCKS_PER_SEC;

        inicio = clock();
        int tam_gul = cobertura_gulosa(&g, cobertura);
        double tempo_gul = 1000.0 * (clock() - inicio) / CLOCKS_PER_SEC;

        printf("  %2i | %7i | %6i | %16li | %13.3f | %7i | %14.3f\n",
               n, contar_arestas(&g), tam_bt, chamadas, tempo_bt, tam_gul, tempo_gul);
    }

    return 0;
}
