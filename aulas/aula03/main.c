#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_lista.h"
#include "busca_grafo.h"

int main(){
    GrafoLista *grafo = criar_grafo(5);

    adicionar_aresta(grafo, 0, 1);
    adicionar_aresta(grafo, 0, 2);
    adicionar_aresta(grafo, 1, 3);
    adicionar_aresta(grafo, 2, 3);
    adicionar_aresta(grafo, 3, 4);
    adicionar_aresta(grafo, 4, 5);

    for (int i = 0; i < grafo->num_vertices; i++)
    {
        printf("%i: -> ", i+1);
        No *no = grafo->lista[i];
        while (no != NULL)
        {
            printf("%i -> ", no->vertice+1);
            no = no->proximo;
        }
        printf("NULL\n");
    }


    int pilha[10];
    int visitado[10];


    memset(visitado, 0, sizeof(visitado));
    dfs(grafo,0, pilha, visitado);
    printf("\n");
    
    memset(visitado, 0, sizeof(visitado));
    dfs(grafo,1, pilha, visitado);
    printf("\n");
    
    memset(visitado, 0, sizeof(visitado));
    dfs(grafo,2, pilha, visitado);
    printf("\n");
    
    memset(visitado, 0, sizeof(visitado));
    dfs(grafo,3, pilha, visitado);
    printf("\n");
    
    memset(visitado, 0, sizeof(visitado));
    dfs(grafo,4, pilha, visitado);
    printf("\n");


    return 0;
} 