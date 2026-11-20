#include "grafo_lista_ponderado.h"

#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

// Retorna 1 (sucesso) ou 0 (existe ciclo negativo alcancavel a partir da origem)
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

// Usar depois que bellman_ford retornar 0: encontra e imprime o ciclo negativo
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred);

#endif
