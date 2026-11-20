#ifndef COBERTURA_H
#define COBERTURA_H

#define MAX_VERTICES 20

typedef struct
{
    int num_vertices;
    int adj[MAX_VERTICES][MAX_VERTICES];
} Grafo;

void inicializar_grafo(Grafo *g, int n);
void adicionar_aresta(Grafo *g, int u, int v);
int contar_arestas(Grafo *g);

// cobertura[v] = 1 se v esta na cobertura, 0 se nao
int eh_cobertura(Grafo *g, int *cobertura);

// Retornam o tamanho da cobertura encontrada e preenchem cobertura[]
int cobertura_minima_bt(Grafo *g, int *cobertura, long *chamadas);
int cobertura_gulosa(Grafo *g, int *cobertura);

#endif
