#ifndef ESTRUCTURAGRAFO_H
#define ESTRUCTURAGRAFO_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// definir el tipo de dato u32 y GrafoSt
#define MAX_u32 4294967295

typedef unsigned int u32;

typedef struct s_vertice* vertice_t;

struct s_vertice {
  u32 name;  // nombre del vertice
  u32 n_nb;  // grado del vertice
  u32 from;  // desde donde tengo que leer la tabla de adyacendcia
};

typedef struct _GrafoSt {
  u32 n_V;              // numero de vertices (n)
  u32 n_E;              // numero de lados (m)
  u32 D;                // Delta del grafo (Δ)
  vertice_t* vertices;  // estructura del grafo tamaño (n)
  u32* vecinos;         // tam (2m)
} GrafoSt;

#endif