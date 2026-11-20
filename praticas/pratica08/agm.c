#include <stdlib.h>
#include <limits.h>
#include "grafo_lista_ponderado.h"
#include "agm.h"

/* ---------- DSU (Union-Find) ---------- */

DSU *criar_dsu(int n)
{
    DSU *d = (DSU *)malloc(sizeof(DSU));
    d->pai = (int *)malloc(sizeof(int) * n);
    d->rank = (int *)malloc(sizeof(int) * n);
    d->n = n;
    for (int i = 0; i < n; i++)
    {
        d->pai[i] = i;
        d->rank[i] = 0;
    }
    return d;
}

void liberar_dsu(DSU *d)
{
    free(d->pai);
    free(d->rank);
    free(d);
}

// Compressao de caminho: todos no caminho passam a apontar direto para a raiz
int find(DSU *d, int x)
{
    if (d->pai[x] != x)
    {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

// Uniao por rank: a arvore mais baixa fica embaixo da mais alta
// Retorna 1 se uniu, 0 se x e y ja estavam no mesmo conjunto
int unir(DSU *d, int x, int y)
{
    int rx = find(d, x);
    int ry = find(d, y);

    if (rx == ry)
    {
        return 0;
    }

    if (d->rank[rx] < d->rank[ry])
    {
        d->pai[rx] = ry;
    }
    else if (d->rank[rx] > d->rank[ry])
    {
        d->pai[ry] = rx;
    }
    else
    {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }
    return 1;
}

/* ---------- Kruskal: O(E log E) ---------- */

static int comparar_arestas(const void *a, const void *b)
{
    return ((Aresta *)a)->peso - ((Aresta *)b)->peso;
}

int kruskal(GrafoLista *g, Aresta *agm)
{
    int n = g->num_vertices;
    int max_arestas = 0;
    int num_arestas = 0;
    int num_agm = 0;
    int custo = 0;

    for (int u = 0; u < n; u++)
    {
        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            max_arestas++;
        }
    }

    // Cada aresta aparece duas vezes na lista (u -> v e v -> u), pega so u < v
    Aresta *arestas = (Aresta *)malloc(sizeof(Aresta) * (max_arestas / 2 + 1));
    for (int u = 0; u < n; u++)
    {
        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            if (u < no->vertice)
            {
                arestas[num_arestas].u = u;
                arestas[num_arestas].v = no->vertice;
                arestas[num_arestas].peso = no->peso;
                num_arestas++;
            }
        }
    }

    qsort(arestas, num_arestas, sizeof(Aresta), comparar_arestas);

    DSU *d = criar_dsu(n);
    for (int i = 0; i < num_arestas && num_agm < n - 1; i++)
    {
        if (unir(d, arestas[i].u, arestas[i].v))
        {
            agm[num_agm++] = arestas[i];
            custo += arestas[i].peso;
        }
    }

    liberar_dsu(d);
    free(arestas);
    return custo;
}

/* ---------- Heap minimo indexado (usado pelo Prim) ---------- */

typedef struct
{
    int *vertices; // vertices[i] = vertice na posicao i do heap
    int *posicao;  // posicao[v] = onde v esta no heap (-1 se ja saiu)
    int *chave;    // chave[v] = menor peso que liga v a arvore
    int tamanho;
} Heap;

static void trocar(Heap *h, int i, int j)
{
    int vi = h->vertices[i];
    int vj = h->vertices[j];
    h->vertices[i] = vj;
    h->vertices[j] = vi;
    h->posicao[vj] = i;
    h->posicao[vi] = j;
}

static void subir(Heap *h, int i)
{
    while (i > 0)
    {
        int pai = (i - 1) / 2;
        if (h->chave[h->vertices[i]] >= h->chave[h->vertices[pai]])
        {
            break;
        }
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(Heap *h, int i)
{
    while (1)
    {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho && h->chave[h->vertices[esq]] < h->chave[h->vertices[menor]])
        {
            menor = esq;
        }
        if (dir < h->tamanho && h->chave[h->vertices[dir]] < h->chave[h->vertices[menor]])
        {
            menor = dir;
        }
        if (menor == i)
        {
            break;
        }
        trocar(h, i, menor);
        i = menor;
    }
}

static int extrair_min(Heap *h)
{
    int v = h->vertices[0];
    trocar(h, 0, h->tamanho - 1);
    h->tamanho--;
    h->posicao[v] = -1;
    descer(h, 0);
    return v;
}

static void diminuir_chave(Heap *h, int v, int nova_chave)
{
    h->chave[v] = nova_chave;
    subir(h, h->posicao[v]);
}

/* ---------- Prim: O((V + E) log V) ---------- */

int prim(GrafoLista *g, int origem, Aresta *agm)
{
    int n = g->num_vertices;
    int num_agm = 0;
    int custo = 0;

    Heap h;
    h.vertices = (int *)malloc(sizeof(int) * n);
    h.posicao = (int *)malloc(sizeof(int) * n);
    h.chave = (int *)malloc(sizeof(int) * n);
    h.tamanho = n;
    int *pai = (int *)malloc(sizeof(int) * n);

    for (int v = 0; v < n; v++)
    {
        h.vertices[v] = v;
        h.posicao[v] = v;
        h.chave[v] = INT_MAX;
        pai[v] = -1;
    }
    diminuir_chave(&h, origem, 0);

    while (h.tamanho > 0)
    {
        int u = extrair_min(&h);

        if (h.chave[u] == INT_MAX)
        {
            break; // grafo desconexo
        }

        if (pai[u] != -1)
        {
            agm[num_agm].u = pai[u];
            agm[num_agm].v = u;
            agm[num_agm].peso = h.chave[u];
            num_agm++;
            custo += h.chave[u];
        }

        // Atualiza a chave dos vizinhos que ainda estao no heap
        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            int v = no->vertice;
            if (h.posicao[v] != -1 && no->peso < h.chave[v])
            {
                pai[v] = u;
                diminuir_chave(&h, v, no->peso);
            }
        }
    }

    free(h.vertices);
    free(h.posicao);
    free(h.chave);
    free(pai);
    return custo;
}
