#include <limits.h>
#include "hamiltoniano.h"
#include "tps.h"

int custo_rota(InstanciaTSP *t, int *rota)
{
    int total = 0;
    for (int i = 0; i < t->n; i++)
    {
        total += t->custo[rota[i]][rota[(i + 1) % t->n]]; // o ultimo volta ao primeiro
    }
    return total;
}

/* ---------- Forca bruta: (n - 1)! permutacoes ---------- */

typedef struct
{
    InstanciaTSP *t;
    int atual[MAX_VERTICES];
    int *melhor;
    int melhor_custo;
    long permutacoes;
} Busca;

static void trocar(int *v, int i, int j)
{
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

// Gera as permutacoes das posicoes k..n-1 trocando elementos de lugar.
// custo_parcial ja soma as arestas de atual[0] ate atual[k - 1]
static void permutar(Busca *b, int k, int custo_parcial)
{
    int n = b->t->n;

    if (k == n)
    {
        b->permutacoes++;
        int total = custo_parcial + b->t->custo[b->atual[n - 1]][b->atual[0]];
        if (total < b->melhor_custo)
        {
            b->melhor_custo = total;
            for (int i = 0; i < n; i++)
            {
                b->melhor[i] = b->atual[i];
            }
        }
        return;
    }

    for (int i = k; i < n; i++)
    {
        trocar(b->atual, k, i);
        permutar(b, k + 1, custo_parcial + b->t->custo[b->atual[k - 1]][b->atual[k]]);
        trocar(b->atual, k, i); // desfaz a troca
    }
}

int tsp_forca_bruta(InstanciaTSP *t, int *rota, long *permutacoes)
{
    Busca b;
    b.t = t;
    b.melhor = rota;
    b.melhor_custo = INT_MAX;
    b.permutacoes = 0;

    // Origem fixa na cidade 0: girar o ciclo nao muda o custo
    for (int i = 0; i < t->n; i++)
    {
        b.atual[i] = i;
    }

    permutar(&b, 1, 0);

    *permutacoes = b.permutacoes;
    return b.melhor_custo;
}

/* ---------- Vizinho mais proximo: O(n^2) ---------- */

int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota)
{
    int visitado[MAX_VERTICES] = {0};

    rota[0] = origem;
    visitado[origem] = 1;

    for (int i = 1; i < t->n; i++)
    {
        int atual = rota[i - 1];
        int proximo = -1;

        // Vai para a cidade nao visitada mais barata a partir de onde estou
        for (int v = 0; v < t->n; v++)
        {
            if (!visitado[v] && (proximo == -1 || t->custo[atual][v] < t->custo[atual][proximo]))
            {
                proximo = v;
            }
        }

        rota[i] = proximo;
        visitado[proximo] = 1;
    }

    return custo_rota(t, rota);
}

/* ---------- Insercao do mais proximo: O(n^3) ---------- */

int tsp_insercao(InstanciaTSP *t, int *rota)
{
    int n = t->n;
    int na_rota[MAX_VERTICES] = {0};
    int tamanho = 1;

    rota[0] = 0;
    na_rota[0] = 1;

    while (tamanho < n)
    {
        // 1. Escolhe a cidade fora da rota mais perto de alguma cidade da rota
        int k = -1;
        int menor_dist = INT_MAX;
        for (int v = 0; v < n; v++)
        {
            if (na_rota[v])
            {
                continue;
            }
            for (int i = 0; i < tamanho; i++)
            {
                if (t->custo[rota[i]][v] < menor_dist)
                {
                    menor_dist = t->custo[rota[i]][v];
                    k = v;
                }
            }
        }

        // 2. Acha o par vizinho (a, b) da rota onde inserir k aumenta menos o custo:
        //    custo[a][k] + custo[k][b] - custo[a][b]
        int melhor_pos = 0;
        int menor_aumento = INT_MAX;
        for (int i = 0; i < tamanho; i++)
        {
            int a = rota[i];
            int b = rota[(i + 1) % tamanho];
            int aumento = t->custo[a][k] + t->custo[k][b] - t->custo[a][b];
            if (aumento < menor_aumento)
            {
                menor_aumento = aumento;
                melhor_pos = i + 1;
            }
        }

        // 3. Abre espaco e coloca k entre a e b
        for (int i = tamanho; i > melhor_pos; i--)
        {
            rota[i] = rota[i - 1];
        }
        rota[melhor_pos] = k;
        na_rota[k] = 1;
        tamanho++;
    }

    return custo_rota(t, rota);
}

/* ---------- Reducao Ciclo Hamiltoniano -> TSP ---------- */

// O grafo tem ciclo hamiltoniano se e somente se o TSP otimo custa exatamente n
// (so usou arestas de custo 1, ou seja, arestas que existem no grafo)
void grafo_para_tsp(Grafo *g, InstanciaTSP *t)
{
    t->n = g->num_vertices;
    for (int u = 0; u < t->n; u++)
    {
        for (int v = 0; v < t->n; v++)
        {
            if (u == v)
            {
                t->custo[u][v] = 0;
            }
            else
            {
                t->custo[u][v] = g->adj[u][v] ? 1 : 2;
            }
        }
    }
}
