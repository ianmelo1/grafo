#include <stdlib.h>
#include <stdio.h>
#include "grafo_lista.h"


int topo = 0;

void dfs(GrafoLista *g, int u,  int *visitado) {
    visitado[u] = 1;
    printf("Empilha %i, Vistia %i\n", u+1, u+1);
    No *no = g->lista[u];
    while (no != NULL)
    {
        int v = no->vertice;
        if (!visitado[v]) dfs(g, v, visitado);
        no = no->proximo;
        
    }
    topo--;
    printf("Desempilha %i\n", u+1);
}