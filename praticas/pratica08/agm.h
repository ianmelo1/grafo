#include "grafo_lista_ponderado.h"

#ifndef AGM_H
#define AGM_H

typedef struct
{
    int *pai, *rank;
    int n;
} DSU;

typedef struct
{
    int u, v, peso;
} Aresta;

DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);

// Preenchem agm[] com as n - 1 arestas escolhidas e retornam o custo total
int kruskal(GrafoLista *g, Aresta *agm);
int prim(GrafoLista *g, int origem, Aresta *agm);

#endif
