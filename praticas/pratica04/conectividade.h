#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#include "grafo_lista.h"

void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo,
                       int *eh_articulacao);

void detectar_pontes(GrafoLista *g, int u, int pai, int *visitado,
                      int *descoberta, int *low, int *tempo,
                      int origens[], int destinos[], int *num_pontes);

#endif
