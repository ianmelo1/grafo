#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static void imprimir_sequencia(int *seq, int n, int fechar)
{
    for (int i = 0; i < n; i++)
    {
        printf(i == 0 ? "%c" : " -> %c", 'A' + seq[i]);
    }
    if (fechar)
    {
        printf(" -> %c", 'A' + seq[0]);
    }
    printf("\n");
}

static void analisar(const char *nome, Grafo *g)
{
    int seq[MAX_VERTICES];
    int n = g->num_vertices;

    printf("%s\n", nome);

    printf("  Caminho hamiltoniano: ");
    if (encontrar_caminho_hamiltoniano_bt(g, seq))
    {
        imprimir_sequencia(seq, n, 0);
    }
    else
    {
        printf("nao existe\n");
    }

    printf("  Ciclo hamiltoniano:   ");
    if (encontrar_ciclo_hamiltoniano_bt(g, seq))
    {
        imprimir_sequencia(seq, n, 1);
    }
    else
    {
        printf("nao existe\n");
    }

    // Reducao: tem ciclo hamiltoniano <=> TSP otimo custa exatamente n
    InstanciaTSP t;
    long permutacoes;
    grafo_para_tsp(g, &t);
    int custo = tsp_forca_bruta(&t, seq, &permutacoes);
    printf("  Reducao para TSP:     custo otimo %i (n = %i) -> %s\n\n",
           custo, n, custo == n ? "tem ciclo" : "nao tem ciclo");
}

static void gerar_tsp_aleatorio(InstanciaTSP *t, int n)
{
    t->n = n;
    for (int u = 0; u < n; u++)
    {
        t->custo[u][u] = 0;
        for (int v = u + 1; v < n; v++)
        {
            t->custo[u][v] = 1 + rand() % 100; // custo entre 1 e 100
            t->custo[v][u] = t->custo[u][v];
        }
    }
}

static double ms_desde(clock_t inicio)
{
    return 1000.0 * (clock() - inicio) / CLOCKS_PER_SEC;
}

int main()
{
    Grafo g;

    // "Casinha": quadrado A-B-C-D com telhado E ligado a A e B
    inicializar_grafo(&g, 5);
    adicionar_aresta(&g, 0, 1);
    adicionar_aresta(&g, 1, 2);
    adicionar_aresta(&g, 2, 3);
    adicionar_aresta(&g, 3, 0);
    adicionar_aresta(&g, 0, 4);
    adicionar_aresta(&g, 1, 4);
    analisar("Grafo 1: casinha (A-B-C-D-A + telhado E em A e B)", &g);

    // A so tem um vizinho: da para passar por todos, mas nao da para voltar
    inicializar_grafo(&g, 4);
    adicionar_aresta(&g, 0, 1);
    adicionar_aresta(&g, 1, 2);
    adicionar_aresta(&g, 2, 3);
    adicionar_aresta(&g, 1, 3);
    analisar("Grafo 2: A-B, B-C, C-D, B-D", &g);

    // Estrela: B, C e D so se ligam ao centro A
    inicializar_grafo(&g, 4);
    adicionar_aresta(&g, 0, 1);
    adicionar_aresta(&g, 0, 2);
    adicionar_aresta(&g, 0, 3);
    analisar("Grafo 3: estrela com centro A", &g);

    // Experimento TSP: forca bruta x heuristicas
    int tamanhos[] = {5, 8, 10, 12};
    int rota[MAX_VERTICES];
    InstanciaTSP t;
    srand(42); // semente fixa: sempre as mesmas instancias

    printf("Experimento TSP (custos aleatorios entre 1 e 100)\n");
    printf("   n | FB custo | FB permutacoes | FB tempo (ms) | VMP custo | VMP tempo (ms) | Ins custo | Ins tempo (ms)\n");
    printf("-----+----------+----------------+---------------+-----------+----------------+-----------+---------------\n");

    for (int i = 0; i < 4; i++)
    {
        int n = tamanhos[i];
        gerar_tsp_aleatorio(&t, n);

        long permutacoes;
        clock_t inicio = clock();
        int custo_fb = tsp_forca_bruta(&t, rota, &permutacoes);
        double tempo_fb = ms_desde(inicio);

        inicio = clock();
        int custo_vmp = tsp_vizinho_mais_proximo(&t, 0, rota);
        double tempo_vmp = ms_desde(inicio);

        inicio = clock();
        int custo_ins = tsp_insercao(&t, rota);
        double tempo_ins = ms_desde(inicio);

        printf("  %2i | %8i | %14li | %13.3f | %9i | %14.3f | %9i | %14.3f\n",
               n, custo_fb, permutacoes, tempo_fb, custo_vmp, tempo_vmp, custo_ins, tempo_ins);
    }

    return 0;
}
