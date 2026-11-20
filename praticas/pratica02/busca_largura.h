#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "grafo_lista.h"

/* Fila (FIFO) com array circular, usada pela BFS */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

Fila *criar_fila(int capacidade);
int fila_vazia(Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);
void liberar_fila(Fila *f);

void bfs(GrafoLista *g, int origem, int *dist, int *pred);
int eh_bipartido(GrafoLista *g);
int contar_componentes(GrafoLista *g);

#endif
