#include "cobertura.h"

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

int contar_arestas(Grafo *g)
{
    int total = 0;
    for (int u = 0; u < g->num_vertices; u++)
    {
        for (int v = u + 1; v < g->num_vertices; v++)
        {
            total += g->adj[u][v];
        }
    }
    return total;
}

// Toda aresta (u, v) precisa ter pelo menos uma ponta na cobertura
int eh_cobertura(Grafo *g, int *cobertura)
{
    for (int u = 0; u < g->num_vertices; u++)
    {
        for (int v = u + 1; v < g->num_vertices; v++)
        {
            if (g->adj[u][v] && !cobertura[u] && !cobertura[v])
            {
                return 0;
            }
        }
    }
    return 1;
}

/* ---------- Forca bruta com backtracking: O(2^n) no pior caso ---------- */

typedef struct
{
    Grafo *g;
    int *atual;      // decisao parcial: 1 = dentro, 0 = fora
    int *melhor;     // melhor cobertura encontrada ate agora
    int melhor_tam;  // tamanho dela (o limite usado na poda)
    long chamadas;   // quantos nos da arvore de busca foram visitados
} Busca;

// Decide o vertice i: primeiro tenta coloca-lo DENTRO, depois FORA
static void backtrack(Busca *b, int i, int tamanho)
{
    b->chamadas++;

    // Poda por limite: ja usei tantos vertices quanto a melhor solucao,
    // entao este ramo nunca vai dar uma cobertura menor
    if (tamanho >= b->melhor_tam)
    {
        return;
    }

    // Todos decididos e nenhuma aresta ficou descoberta: achei uma melhor
    if (i == b->g->num_vertices)
    {
        b->melhor_tam = tamanho;
        for (int v = 0; v < b->g->num_vertices; v++)
        {
            b->melhor[v] = b->atual[v];
        }
        return;
    }

    // Ramo 1: i entra na cobertura
    b->atual[i] = 1;
    backtrack(b, i + 1, tamanho + 1);

    // Ramo 2: i fica fora. So vale se nenhum vizinho ja decidido ficou fora,
    // senao a aresta entre eles fica descoberta (poda por inviabilidade)
    for (int j = 0; j < i; j++)
    {
        if (b->g->adj[i][j] && !b->atual[j])
        {
            b->atual[i] = 0;
            return;
        }
    }
    b->atual[i] = 0;
    backtrack(b, i + 1, tamanho);
}

int cobertura_minima_bt(Grafo *g, int *cobertura, long *chamadas)
{
    int atual[MAX_VERTICES];
    Busca b;
    b.g = g;
    b.atual = atual;
    b.melhor = cobertura;
    b.chamadas = 0;

    // Limite inicial: todos os vertices sempre formam uma cobertura
    b.melhor_tam = g->num_vertices;
    for (int v = 0; v < g->num_vertices; v++)
    {
        cobertura[v] = 1;
    }

    backtrack(&b, 0, 0);

    *chamadas = b.chamadas;
    return b.melhor_tam;
}

/* ---------- Heuristica gulosa (maior grau primeiro): O(n^3) ---------- */

int cobertura_gulosa(Grafo *g, int *cobertura)
{
    int n = g->num_vertices;
    int restante[MAX_VERTICES][MAX_VERTICES]; // arestas ainda nao cobertas
    int tamanho = 0;

    for (int u = 0; u < n; u++)
    {
        cobertura[u] = 0;
        for (int v = 0; v < n; v++)
        {
            restante[u][v] = g->adj[u][v];
        }
    }

    while (1)
    {
        // Acha o vertice que cobre mais arestas ainda descobertas
        int escolhido = -1;
        int maior_grau = 0;
        for (int u = 0; u < n; u++)
        {
            int grau = 0;
            for (int v = 0; v < n; v++)
            {
                grau += restante[u][v];
            }
            if (grau > maior_grau)
            {
                maior_grau = grau;
                escolhido = u;
            }
        }

        if (escolhido == -1)
        {
            break; // nao sobrou aresta descoberta
        }

        // Coloca na cobertura e apaga todas as arestas dele
        cobertura[escolhido] = 1;
        tamanho++;
        for (int v = 0; v < n; v++)
        {
            restante[escolhido][v] = 0;
            restante[v][escolhido] = 0;
        }
    }

    return tamanho;
}
