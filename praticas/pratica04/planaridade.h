#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include "grafo_lista.h"

int eh_planar_euler(GrafoLista *g);
int contem_k5(GrafoLista *g);
int contem_k33(GrafoLista *g);
int eh_planar(GrafoLista *g);

#endif
