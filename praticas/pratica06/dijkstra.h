#include "grafo_lista_ponderado.h"

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

typedef struct
{
    int vertice;
    int distancia;
} NoHeap;

typedef struct
{
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *h);
int heap_vazio(MinHeap *h);
void inserir(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif
