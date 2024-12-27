#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "AlduinPaarthurnaxIrileth.h"
#include "AniquilamientoPositronicoIonizanteGravitatorio.h"

int main() {
    Grafo G;
    G = ConstruccionDelGrafo();
    printf("El Nombre del i=2 es %u\n", Nombre(2, G));
    printf("El Grado del i=2 es %u\n", Grado(2, G));
    printf("El Vecino j=0, del Vertice k=0, tiene indice %u\n",
           IndiceONVecino(0, 0, G));

    u32 n = NumeroDeVertices(G);
    // orden natural
    u32* OrdenNatural = calloc(n, sizeof(u32));
    u32* Coloreo = calloc(n, sizeof(u32));

    for (u32 i = 0; i < n; i++) {
        OrdenNatural[i] = i;
    }

    u32 color = Greedy(G, OrdenNatural, Coloreo);

    u32* Key = calloc(n, sizeof(u32));
    AleatorizarKeys(n, 0, Key);
    OrdenFromKey(n, Key, OrdenNatural);
    printf("key:");
    for (u32 i = 1; i < n; i++) {
        printf("%d,", Key[i]);
        assert(Key[OrdenNatural[i - 1]] >= Key[OrdenNatural[i]]);
    }
    printf("\nn es: %u\n",n);
    printf("\nGreedy da color: %u\n", color);

    u32* nuevoColor = PermutarColores(n, Coloreo, 3);
    nuevoColor[1] = nuevoColor[1]; // para que no se queje el comp


    u32* nuevisimoColor = RecoloreoCardinalidadDecrecienteBC(n, Coloreo);
    nuevisimoColor[0] = nuevisimoColor[0]; // para que no se queje el comp


    u32* elpatito = Bipartito(G);
    if (elpatito != NULL) {
        printf("\nEs Bipartito\n");
        free(elpatito);
    } else {
        printf("No es Bipartito\n");
    }
    free(nuevisimoColor);
    free(nuevoColor);
    free(Key);
    free(OrdenNatural);
    free(Coloreo);
    DestruccionDelGrafo(G);
    return 0;
}
