#include "hamiltoniano.h"

#ifndef TPS_H
#define TPS_H

// Grafo completo com custo entre todo par de cidades
typedef struct
{
    int n;
    int custo[MAX_VERTICES][MAX_VERTICES];
} InstanciaTSP;

// Todas preenchem rota[] com as n cidades (a volta para rota[0] fica implicita)
// e retornam o custo total do ciclo
int tsp_forca_bruta(InstanciaTSP *t, int *rota, long *permutacoes);
int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota);
int tsp_insercao(InstanciaTSP *t, int *rota);

int custo_rota(InstanciaTSP *t, int *rota);

// Reducao: aresta existente custa 1, inexistente custa 2
void grafo_para_tsp(Grafo *g, InstanciaTSP *t);

#endif
