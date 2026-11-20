#include "planaridade.h"

/* Condicao necessaria (mas nao suficiente) de planaridade: todo grafo
   planar simples com n >= 3 vertices respeita m <= 3n - 6. Se isso
   falhar, o grafo certamente NAO e planar. */
int eh_planar_euler(GrafoLista *g) {
    int n = g->n;
    if (n < 3) return 1;

    int soma_graus = 0;
    for (int u = 0; u < n; u++) {
        soma_graus += grau_lista(g, u);
    }
    int m = soma_graus / 2; /* cada aresta foi contada nos dois sentidos */

    return m <= 3 * n - 6;
}

static void buscar_k5(GrafoLista *g, int *escolhidos, int inicio, int quantidade, int *encontrado) {
    if (*encontrado) return;

    if (quantidade == 5) {
        int completo = 1;
        for (int i = 0; i < 5 && completo; i++) {
            for (int j = i + 1; j < 5 && completo; j++) {
                if (!sao_adjacentes_lista(g, escolhidos[i], escolhidos[j])) {
                    completo = 0;
                }
            }
        }
        if (completo) *encontrado = 1;
        return;
    }

    for (int v = inicio; v < g->n; v++) {
        escolhidos[quantidade] = v;
        buscar_k5(g, escolhidos, v + 1, quantidade + 1, encontrado);
        if (*encontrado) return;
    }
}

/* Forca bruta: tenta achar 5 vertices em que todos os 10 pares sejam
   adjacentes entre si (um K5 como subgrafo). So e viavel para n <= 10. */
int contem_k5(GrafoLista *g) {
    if (g->n < 5 || g->n > 10) return 0;

    int escolhidos[5];
    int encontrado = 0;
    buscar_k5(g, escolhidos, 0, 0, &encontrado);
    return encontrado;
}

static int contar_bits(int x) {
    int c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

static int bipartido_completo(GrafoLista *g, int *vertices, int mascara) {
    /* bit i = 1 -> vertices[i] esta no grupo A; bit i = 0 -> grupo B */
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            int i_no_a = (mascara >> i) & 1;
            int j_no_a = (mascara >> j) & 1;
            if (i_no_a != j_no_a && !sao_adjacentes_lista(g, vertices[i], vertices[j])) {
                return 0;
            }
        }
    }
    return 1;
}

static void buscar_k33(GrafoLista *g, int *vertices, int inicio, int quantidade, int *encontrado) {
    if (*encontrado) return;

    if (quantidade == 6) {
        for (int mascara = 0; mascara < 64 && !*encontrado; mascara++) {
            if (contar_bits(mascara) == 3 && bipartido_completo(g, vertices, mascara)) {
                *encontrado = 1;
            }
        }
        return;
    }

    for (int v = inicio; v < g->n; v++) {
        vertices[quantidade] = v;
        buscar_k33(g, vertices, v + 1, quantidade + 1, encontrado);
        if (*encontrado) return;
    }
}

/* Forca bruta: tenta achar 6 vertices divisiveis em dois grupos de 3 com
   todas as 9 ligacoes cruzadas presentes (um K3,3 como subgrafo). So e
   viavel para n <= 10. */
int contem_k33(GrafoLista *g) {
    if (g->n < 6 || g->n > 10) return 0;

    int vertices[6];
    int encontrado = 0;
    buscar_k33(g, vertices, 0, 0, &encontrado);
    return encontrado;
}

/* Heuristica de planaridade (Euler + Kuratowski simplificado): encontrar
   K5/K3,3 como subgrafo ou violar a condicao de Euler PROVA que o grafo
   nao e planar. O teorema de Kuratowski original exige checar
   SUBDIVISOES de K5/K3,3 (que podem existir sem um K5/K3,3 literal como
   subgrafo); aqui buscamos apenas o caso mais direto, entao nao encontrar
   nada e um indicio de planaridade, nao uma prova completa. */
int eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g)) return 0;
    if (contem_k5(g)) return 0;
    if (contem_k33(g)) return 0;
    return 1;
}
