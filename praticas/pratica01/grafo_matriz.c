#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

GrafoMatriz *criar_grafo_matriz(int n) {
    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        g->adj[i] = calloc(n, sizeof(int));
    }
    return g;
}

void inserir_aresta_matriz(GrafoMatriz *g, int origem, int destino) {
    if (origem < 0 || origem >= g->n || destino < 0 || destino >= g->n) return;
    g->adj[origem][destino] = 1;
    g->adj[destino][origem] = 1;
}

void remover_aresta_matriz(GrafoMatriz *g, int origem, int destino) {
    if (origem < 0 || origem >= g->n || destino < 0 || destino >= g->n) return;
    g->adj[origem][destino] = 0;
    g->adj[destino][origem] = 0;
}

int grau_matriz(GrafoMatriz *g, int v) {
    if (v < 0 || v >= g->n) return -1;
    int grau = 0;
    for (int i = 0; i < g->n; i++) {
        if (g->adj[v][i]) grau++;
    }
    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return 0;
    return g->adj[u][v];
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}