#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo_lista_ponderado.h"
#include "dijkstra.h"

/* ---------- Min-Heap ---------- */

MinHeap *criar_min_heap(int capacidade)
{
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    h->dados = (NoHeap *)malloc(sizeof(NoHeap) * capacidade);
    h->posicao = (int *)malloc(sizeof(int) * capacidade);
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++)
    {
        h->posicao[i] = -1; // -1 = vertice fora do heap
    }
    return h;
}

void liberar_min_heap(MinHeap *h)
{
    free(h->dados);
    free(h->posicao);
    free(h);
}

int heap_vazio(MinHeap *h)
{
    return h->tamanho == 0;
}

// Troca dois nos de lugar e atualiza o mapa posicao[]
static void trocar(MinHeap *h, int i, int j)
{
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i)
{
    while (i > 0)
    {
        int pai = (i - 1) / 2;
        if (h->dados[i].distancia >= h->dados[pai].distancia)
        {
            break;
        }
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i)
{
    while (1)
    {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
        {
            menor = esq;
        }
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
        {
            menor = dir;
        }
        if (menor == i)
        {
            break;
        }
        trocar(h, i, menor);
        i = menor;
    }
}

// O(log V): coloca no fim e sobe ate a posicao certa
void inserir(MinHeap *h, int vertice, int distancia)
{
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
}

// O(log V): tira a raiz, poe o ultimo no lugar e desce
NoHeap extrair_minimo(MinHeap *h)
{
    NoHeap minimo = h->dados[0];
    trocar(h, 0, h->tamanho - 1);
    h->tamanho--;
    h->posicao[minimo.vertice] = -1;
    descer(h, 0);
    return minimo;
}

// O(log V): acha o vertice pelo posicao[] e sobe
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia)
{
    int i = h->posicao[vertice];
    h->dados[i].distancia = nova_distancia;
    subir(h, i);
}

/* ---------- Dijkstra: O((V + E) log V) ---------- */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    MinHeap *h = criar_min_heap(g->num_vertices);

    for (int v = 0; v < g->num_vertices; v++)
    {
        dist[v] = INT_MAX;
        pred[v] = -1;
    }
    dist[origem] = 0;
    inserir(h, origem, 0);

    while (!heap_vazio(h))
    {
        int u = extrair_minimo(h).vertice;

        // Relaxacao: passar por u deixa o caminho ate v mais curto?
        for (No *no = g->lista[u]; no != NULL; no = no->proximo)
        {
            int v = no->vertice;
            int nova = dist[u] + no->peso;
            if (nova < dist[v])
            {
                if (dist[v] == INT_MAX)
                {
                    inserir(h, v, nova); // primeira vez que v e alcancado
                }
                else
                {
                    diminuir_chave(h, v, nova);
                }
                dist[v] = nova;
                pred[v] = u;
            }
        }
    }

    liberar_min_heap(h);
}

// Volta pelo pred[] ate a origem e imprime na ordem certa (recursao)
void reconstruir_caminho(int origem, int destino, int *pred)
{
    if (destino == origem)
    {
        printf("%c", 'A' + origem);
        return;
    }
    if (pred[destino] == -1)
    {
        printf("sem caminho");
        return;
    }
    reconstruir_caminho(origem, pred[destino], pred);
    printf(" -> %c", 'A' + destino);
}
