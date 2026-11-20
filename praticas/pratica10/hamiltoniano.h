#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#define MAX_VERTICES 20

typedef struct
{
    int num_vertices;
    int adj[MAX_VERTICES][MAX_VERTICES];
} Grafo;

void inicializar_grafo(Grafo *g, int n);
void adicionar_aresta(Grafo *g, int u, int v);

// Retornam 1 se encontraram e preenchem caminho[] / ciclo[] com os n vertices
int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho);
int encontrar_ciclo_hamiltoniano_bt(Grafo *g, int *ciclo);

#endif
