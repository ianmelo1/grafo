#include "hamiltoniano.h"

void inicializar_grafo(Grafo *g, int n)
{
    g->num_vertices = n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            g->adj[i][j] = 0;
        }
    }
}

void adicionar_aresta(Grafo *g, int u, int v)
{
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

// Monta o caminho uma posicao por vez, so andando por arestas que existem.
// Quando um vertice nao leva a lugar nenhum, desfaz a escolha (backtracking)
static int backtrack(Grafo *g, int *caminho, int *visitado, int pos, int fechar_ciclo)
{
    int n = g->num_vertices;

    if (pos == n)
    {
        // Passou por todos. Para ser ciclo, o ultimo precisa voltar ao primeiro
        return !fechar_ciclo || g->adj[caminho[n - 1]][caminho[0]];
    }

    int anterior = caminho[pos - 1];
    for (int v = 0; v < n; v++)
    {
        if (!visitado[v] && g->adj[anterior][v])
        {
            visitado[v] = 1;
            caminho[pos] = v;

            if (backtrack(g, caminho, visitado, pos + 1, fechar_ciclo))
            {
                return 1;
            }

            visitado[v] = 0; // desfaz e tenta o proximo vizinho
        }
    }

    return 0;
}

static int buscar(Grafo *g, int *caminho, int inicio, int fechar_ciclo)
{
    int visitado[MAX_VERTICES] = {0};

    caminho[0] = inicio;
    visitado[inicio] = 1;
    return backtrack(g, caminho, visitado, 1, fechar_ciclo);
}

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho)
{
    // O caminho pode comecar em qualquer vertice
    for (int inicio = 0; inicio < g->num_vertices; inicio++)
    {
        if (buscar(g, caminho, inicio, 0))
        {
            return 1;
        }
    }
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(Grafo *g, int *ciclo)
{
    if (g->num_vertices < 3)
    {
        return 0; // ciclo simples precisa de pelo menos 3 vertices
    }

    // Um ciclo passa por todos os vertices, entao basta comecar no 0
    return buscar(g, ciclo, 0, 1);
}
