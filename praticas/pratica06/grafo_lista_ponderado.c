#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista_ponderado.h"

GrafoListaPonderado *criar_grafo(int n)
{
    GrafoListaPonderado *g = (GrafoListaPonderado *)malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = n;
    g->lista = (No **)malloc(sizeof(No *) * n);
    for (int i = 0; i < n; i++)
    {
        g->lista[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int p)
{
    // u -> v
    No *proximo = g->lista[u];
    No *no = (No *)malloc(sizeof(No));
    no->vertice = v;
    no->peso = p;
    no->proximo = proximo;
    g->lista[u] = no;

    // v -> u
    proximo = g->lista[v];
    no = (No *)malloc(sizeof(No));
    no->vertice = u;
    no->peso = p;
    no->proximo = proximo;
    g->lista[v] = no;
}

void adicionar_arco(GrafoListaPonderado *g, int u, int v, int p)
{
    // u -> v
    No *proximo = g->lista[u];
    No *no = (No *)malloc(sizeof(No));
    no->vertice = v;
    no->peso = p;
    no->proximo = proximo;
    g->lista[u] = no;
}

void imprimir_grafo(GrafoListaPonderado *g)
{
    printf("\n");
    for (int i = 0; i < g->num_vertices; i++)
    {
        printf("%i: -> ", i + 1);
        No *no = g->lista[i];
        while (no != NULL)
        {
            printf("(%i, %i) -> ", no->vertice + 1, no->peso);
            no = no->proximo;
        }
        printf("NULL\n");
    }
    printf("\n");
}

void liberar_grafo(GrafoListaPonderado *g)
{
    for (int i = 0; i < g->num_vertices; i++)
    {
        No *no = g->lista[i];
        while (no != NULL)
        {
            No *proximo = no->proximo;
            free(no);
            no = proximo;
        }
    }
    free(g->lista);
    free(g);
}
