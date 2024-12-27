#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "AniquilamientoPositronicoIonizanteGravitatorio.h"
#include "EstructuraGrafo.h"

// asumo lineas de hasta 512 TEMP_SIZE caracteres
#define TEMP_SIZE 512

typedef struct {
    u32 v1;
    u32 v2;
} edge;

static int cmpfunc(const void* a, const void* b) {
    const edge *p = a, *q = b;
    if (p->v1 < q->v1) {
        return -1;
    }
    if (p->v1 > q->v1) {
        return 1;
    }
    if (p->v2 < q->v2) {
        return -1;
    }
    if (p->v2 > q->v2) {
        return 1;
    }
    fprintf(stderr, "hay un lado repetido");
    return 0;
}

static edge* parse_file(edge* lista_de_lados, Grafo G) {
    char temp[TEMP_SIZE];
    bool encontre_p_edge = false;
    u32 cant_e = 0;
    char letter[1], compare[4];
    u32 vertex_name_a, vertex_name_b;
    char primer_letra = ' ';

    // cuidado, apovecho el lazy evaluation para que no se ejecute el fgets
    while (!encontre_p_edge && fgets(temp, TEMP_SIZE, stdin) != NULL) {
        // fgets se detiene cuando se termina el buffer, EOF o newline
        primer_letra = temp[0];

        if (primer_letra == 'p') {
            sscanf(temp, "%s %s %u %u", letter, compare, &(G->n_V), &(G->n_E));
            if (!strcmp("edge", compare)) {
                encontre_p_edge = true;
            }

        } else if (primer_letra != 'c') {
            fprintf(stderr, "Linea del archivo no esta bien formateado\n");
        }
    }

    lista_de_lados = calloc(G->n_E * 2, sizeof(edge));

    while (cant_e < G->n_E && fgets(temp, TEMP_SIZE, stdin) != NULL) {
        primer_letra = temp[0];
        if (primer_letra == 'e') {
            sscanf(temp, "%s %u %u", letter, &vertex_name_a, &vertex_name_b);
            // pongo el vertice 1~2
            lista_de_lados[cant_e * 2].v1 = vertex_name_a;
            lista_de_lados[cant_e * 2].v2 = vertex_name_b;
            // pongo el veritce 2~1
            lista_de_lados[cant_e * 2 + 1].v2 = vertex_name_a;
            lista_de_lados[cant_e * 2 + 1].v1 = vertex_name_b;
            cant_e++;
        } else if (primer_letra != 'c') {
            fprintf(stderr, "Linea del archivo no esta bien formateado\n");
        }
    }

    if (G->n_E != cant_e) {
        fprintf(stderr, "Hay menos cantidad de lados que en la linea p edge\n");
        free(lista_de_lados);
        return NULL;
    }

    qsort(lista_de_lados, G->n_E * 2, sizeof(edge), cmpfunc);
    // lista ordenada!!!
    return lista_de_lados;
}

Grafo ConstruccionDelGrafo() {
    u32* array_vecinos = NULL;
    edge* lista_de_lados = NULL;
    Grafo G = calloc(1, sizeof(GrafoSt));
    if (G == NULL) {
        printf("Error memory\n");
        exit(EXIT_FAILURE);
    }
    G->n_V = 0;
    G->n_E = 0;
    G->D = 0;

    // utilizar el input para asignar los valores n_V, n_E , luego calcular el
    // delta, y formar la estructura del grafo
    // deja listo para cargar los vertices y carga G->n_E y G->n_V
    lista_de_lados = parse_file(lista_de_lados, G);
    if (lista_de_lados == NULL) {
        free(G);
        return NULL;
    }
    edge* lista_de_lados_adyacentes = calloc(G->n_E * 2, sizeof(edge));

    // la lista de adyacencia esta ordenada en los v2
    // asumo que el grafo por lo menos tiene un lado
    array_vecinos = calloc((G->n_E * 2), sizeof(u32));
    G->vertices = calloc(G->n_V, sizeof(struct s_vertice));

    G->vertices[0] = calloc(1, sizeof(struct s_vertice));
    G->vertices[0]->name = lista_de_lados[0].v1;
    G->vertices[0]->n_nb = 1;
    G->vertices[0]->from = 0;
    u32 cant_vertices_cargados = 0;
    // cargo en lista_de_lados_adyacentes lista_de lados pero al revez y en vez
    // de usar los lados uso la cant_de_vertices_cargados para que al ordenarlo
    // me quede la lista de adyacencia
    lista_de_lados_adyacentes[0].v1 = lista_de_lados[0].v2;
    lista_de_lados_adyacentes[0].v2 = cant_vertices_cargados;

    for (u32 i = 1; i < (G->n_E * 2); i++) {
        if (lista_de_lados[i - 1].v1 == lista_de_lados[i].v1) {
            G->vertices[cant_vertices_cargados]->n_nb++;
        } else {
            cant_vertices_cargados++;
            lista_de_lados_adyacentes[i].v1 = cant_vertices_cargados;
            G->D = G->D > G->vertices[cant_vertices_cargados - 1]->n_nb
                       ? G->D
                       : G->vertices[cant_vertices_cargados - 1]->n_nb;
            G->vertices[cant_vertices_cargados] =
                calloc(1, sizeof(struct s_vertice));
            G->vertices[cant_vertices_cargados]->name = lista_de_lados[i].v1;
            G->vertices[cant_vertices_cargados]->n_nb = 1;
            G->vertices[cant_vertices_cargados]->from = i;
        }
        lista_de_lados_adyacentes[i].v1 = lista_de_lados[i].v2;
        lista_de_lados_adyacentes[i].v2 = cant_vertices_cargados;
    }
    G->D = G->D > G->vertices[cant_vertices_cargados]->n_nb
               ? G->D
               : G->vertices[cant_vertices_cargados]->n_nb;

    qsort(lista_de_lados_adyacentes, G->n_E * 2, sizeof(edge), cmpfunc);

    for (u32 i = 0; i < (G->n_E * 2); i++) {
        array_vecinos[i] = lista_de_lados_adyacentes[i].v2;
    }
    G->vecinos = array_vecinos;
    free(lista_de_lados);
    free(lista_de_lados_adyacentes);

    return G;
}

void DestruccionDelGrafo(Grafo G) {
    if (G->vertices) {
        for (u32 i = 0; i < G->n_V; i++) {
            if (G->vertices[i]) {
                G->vertices[i]->from = 0;
                G->vertices[i]->name = 0;
                G->vertices[i]->n_nb = 0;
                free(G->vertices[i]);
                G->vertices[i] = NULL;
            }
        }
        free(G->vertices);
    }

    if (G->vecinos) {
        free(G->vecinos);
    }
    free(G);
    G = NULL;
}

u32 NumeroDeVertices(Grafo G) {
    return G->n_V;
}

u32 NumeroDeLados(Grafo G) {
    return G->n_E;
}

u32 Delta(Grafo G) {
    return G->D;
}

u32 Nombre(u32 i, Grafo G) {
    u32 size = G->n_V;
    assert(size - 1 >= i);
    return G->vertices[i]->name;
}

u32 Grado(u32 i, Grafo G) {
    u32 res;
    if (i >= G->n_V) {
        res = MAX_u32;  // error
    } else {
        res = G->vertices[i]->n_nb;
    }
    return res;
}

u32 IndiceONVecino(u32 j, u32 k, Grafo G) {
    u32 ng_j_k, aux;
    if (k < G->n_V && j < Grado(k, G)) {
        aux = G->vertices[k]->from + j;
        ng_j_k = G->vecinos[aux];
    } else {
        ng_j_k = MAX_u32;  // error
    }
    return ng_j_k;
}