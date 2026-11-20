#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo_lista_ponderado.h"
#include "bellman_ford.h"

// Passa por todos os arcos uma vez tentando melhorar dist[]
// Retorna o ultimo vertice que melhorou, ou -1 se nenhum melhorou
static int relaxar_arcos(GrafoListaPonderado *g, int *dist, int *pred)
{
    int ultimo = -1;

    for (int u = 0; u < g->num_vertices; u++)
    {
        if (dist[u] == INT_MAX)
        {
            continue; // u ainda nao foi alcancado, nao da pra sair dele
        }

        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            int v = no->vertice;
            if (dist[u] + no->peso < dist[v])
            {
                dist[v] = dist[u] + no->peso;
                pred[v] = u;
                ultimo = v;
            }
        }
    }

    return ultimo;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    int n = g->num_vertices;

    for (int v = 0; v < n; v++)
    {
        dist[v] = INT_MAX;
        pred[v] = -1;
    }
    dist[origem] = 0;

    // Um caminho minimo simples tem no maximo n - 1 arcos
    for (int i = 0; i < n - 1; i++)
    {
        relaxar_arcos(g, dist, pred);
    }

    // n-esima rodada: se ainda melhorar algo, existe ciclo negativo
    for (int u = 0; u < n; u++)
    {
        if (dist[u] == INT_MAX)
        {
            continue;
        }
        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            if (dist[u] + no->peso < dist[no->vertice])
            {
                return 0;
            }
        }
    }

    return 1;
}

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred)
{
    int n = g->num_vertices;

    // Mais uma rodada de relaxacao para achar um vertice afetado pelo ciclo
    int x = relaxar_arcos(g, dist, pred);
    if (x == -1)
    {
        printf("Nenhum ciclo negativo encontrado\n");
        return;
    }

    // Retrocede n vezes: garante que x caiu dentro do ciclo
    for (int i = 0; i < n; i++)
    {
        x = pred[x];
    }

    // Anda pelo pred[] ate voltar em x (o ciclo sai de tras pra frente)
    int *ciclo = (int *)malloc(sizeof(int) * n);
    int tamanho = 0;
    int v = x;
    do
    {
        ciclo[tamanho++] = v;
        v = pred[v];
    } while (v != x);

    printf("Ciclo negativo: ");
    for (int i = tamanho - 1; i >= 0; i--)
    {
        printf("%c -> ", 'A' + ciclo[i]);
    }
    printf("%c\n", 'A' + ciclo[tamanho - 1]);

    free(ciclo);
}
