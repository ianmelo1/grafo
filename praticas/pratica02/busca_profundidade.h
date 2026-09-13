#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "grafo_lista.h"

/* Pilha (LIFO) com array, disponivel para uma DFS iterativa */
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

Pilha *criar_pilha(int capacidade);
int pilha_vazia(Pilha *p);
void empilhar(Pilha *p, int valor);
int desempilhar(Pilha *p);
void liberar_pilha(Pilha *p);

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo, int *entrada, int *saida);
int tem_ciclo(GrafoLista *g);

#endif
