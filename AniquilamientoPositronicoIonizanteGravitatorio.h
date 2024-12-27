#ifndef __APIG_H
#define __APIG_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "EstructuraGrafo.h"

typedef GrafoSt* Grafo;

Grafo ConstruccionDelGrafo();
void DestruccionDelGrafo(Grafo G);
u32 NumeroDeVertices(Grafo G);
u32 NumeroDeLados(Grafo G);
u32 Delta(Grafo G);

u32 Nombre(u32 i, Grafo G);
u32 Grado(u32 i, Grafo G);
u32 IndiceONVecino(u32 j, u32 k, Grafo G);

#endif