
#include <stdbool.h>
#include <stdlib.h>
#include "AlduinPaarthurnaxIrileth.h"
#include "AniquilamientoPositronicoIonizanteGravitatorio.h"
#include "queue.h"

// esta macro para obtener la cant de bits de un numero en tiempo de compilacion
// la obtuve de aca
// https://stackoverflow.com/questions/3957252/is-there-any-way-to-compute-the-width-of-an-integer-type-at-compile-time
#define IMAX_BITS(m) \
    ((m) / ((m) % 255 + 1) / 255 % 255 * 8 + 7 - 86 / ((m) % 255 + 12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)

u32 rand32(void) {
    u32 r = 0;
    for (int i = 0; i < 32; i += RAND_MAX_WIDTH) {
        r <<= RAND_MAX_WIDTH;
        r ^= (unsigned)rand();
    }
    return r;
}

static bool EsPropio(Grafo G, u32* Coloreo) {
    bool propio = true;
    u32 n = NumeroDeVertices(G);
    u32 indiceTemp = 0;

    for (u32 i = 0; propio && (i < n); i++) {
        // esto no es esctricatamente correcto pero es muy probable que este mal
        // si doy el color maximo
        propio = Coloreo[i] != MAX_u32;
        for (u32 j = 0; propio && (j < Grado(i, G)); j++) {
            indiceTemp = IndiceONVecino(j, i, G);
            propio = (Coloreo[i] != Coloreo[indiceTemp]);
        }
    }
    return propio;
}

u32* Bipartito(Grafo G) {
    u32* array_coloreo;
    u32 n = NumeroDeVertices(G);
    array_coloreo = calloc(n, sizeof(u32));
    if (array_coloreo == NULL) {
        return NULL;
    }

    u32 indiceTemp = 0;
    u32 indiceVecinosTemp = 0;

    for (u32 i = 0; i < n; i++) {
        array_coloreo[i] = MAX_u32;
    }

    // coloreo el grafo en 2m

    struct queue q;
    // Bfs
estoydemente:
    initqueue(&q, n);
    enqueue(&q, indiceTemp);
    array_coloreo[indiceTemp] = 1;

    while (!emptyqcheck(&q)) {
        indiceTemp = dequeue(&q);
        for (u32 j = 0; j < Grado(indiceTemp, G); j++) {
            indiceVecinosTemp = IndiceONVecino(j, indiceTemp, G);
            if (array_coloreo[indiceVecinosTemp] == MAX_u32) {
                array_coloreo[indiceVecinosTemp] =
                    array_coloreo[indiceTemp] % 2 + 1;
                enqueue(&q, indiceVecinosTemp);
            }
        }
    }
    destroyqueue(&q);

    // reviso las componentes no conexas
    for (u32 i = 0; i < n; i++) {
        if (array_coloreo[i] == MAX_u32) {
            indiceTemp = i;
            goto estoydemente;
        }
    }
    // chequeo si es propio
    if (EsPropio(G, array_coloreo)) {
        return array_coloreo;
    } else {
        free(array_coloreo);
        return NULL;
    }
}

static u32 MinimoColorQueNoEsta(bool* coloresIndexados, u32 cantDeColores) {
    for (u32 i = 0; i < cantDeColores; i++)
        if (coloresIndexados[i] == false) {
            return i;
        }

    return cantDeColores;
}

u32 Greedy(Grafo G, u32* Orden, u32* Coloreo) {
    u32 indiceTemp;
    u32 numeroDeColores = 0;
    u32 n = NumeroDeVertices(G);

    u32 grado = 0;
    bool* esta_en_el_vecino = NULL;
    Coloreo[Orden[0]] = 0;

    for (u32 i = 1; i < n; i++) {
        Coloreo[i] = MAX_u32;
    }

    u32 delta = Delta(G);
    esta_en_el_vecino = calloc(delta, sizeof(bool));

    for (u32 i = 1; i < n; i++) {
        grado = Grado(Orden[i], G);
        // inicializo el array
        for (u32 j = 0; j < grado; j++) {
            esta_en_el_vecino[j] = false;
        }
        // marco los colores presentes en los vecinos
        for (u32 j = 0; j < grado; j++) {
            indiceTemp = IndiceONVecino(j, Orden[i], G);

            if (Coloreo[indiceTemp] != MAX_u32) {
                esta_en_el_vecino[Coloreo[indiceTemp]] = true;
            }
        }

        Coloreo[Orden[i]] = MinimoColorQueNoEsta(esta_en_el_vecino, grado);
        // calculo el color maximo parcial
        if (Coloreo[Orden[i]] > numeroDeColores) {
            numeroDeColores = Coloreo[Orden[i]];
        }
    }
    free(esta_en_el_vecino);
    assert(EsPropio(G, Coloreo));  // sacarlo para la entrega
    return numeroDeColores + 1;
}

typedef struct {
    u32 index;
    u32 value;
} tuple;

static int cmpfunc(const void* a, const void* b) {
    const tuple *p = a, *q = b;
    if (p->value > q->value) {
        return -1;
    }
    if (p->value < q->value) {
        return 1;
    }
    return 0;
}

// ordenamiento a partir de clave

char OrdenFromKey(u32 n, u32* key, u32* Orden) {
    tuple* OrdenTemp = calloc(n, sizeof(tuple));
    if (OrdenTemp == NULL) {
        return '1';
    }
    for (u32 i = 0; i < n; i++) {
        OrdenTemp[i].index = i;
        OrdenTemp[i].value = key[i];
    }
    qsort(OrdenTemp, n, sizeof(tuple), cmpfunc);
    for (u32 i = 0; i < n; i++) {
        Orden[i] = OrdenTemp[i].index;
    }
    free(OrdenTemp);

    // debug{
    for (u32 i = 1; i < n; i++) {
        assert(key[Orden[i - 1]] >= key[Orden[i]]);
    }
    // debug}
    return '0';
}

// claves aleatorias

void AleatorizarKeys(u32 n, u32 R, u32* key) {
    srand(R);
    for (u32 i = 0; i < n; i++) {
        key[i] = rand32() % n;
    }
}

static u32 cantColoresEnColoreo(u32* Coloreo, u32 largo) {
    u32 cantColores = 0;
    for (u32 i = 0; i < largo; i++) {
        if (Coloreo[i] > cantColores) {
            cantColores = Coloreo[i];
        }
    }
    cantColores++;
    return cantColores;
}

// permutadores de colores
u32* PermutarColores(u32 n, u32* Coloreo, u32 R) {
    u32* ColoreoNuevo = calloc(n, sizeof(u32));
    if (ColoreoNuevo == NULL) {
        return NULL;
    }

    u32 r = cantColoresEnColoreo(Coloreo, n);

    u32* PermC = calloc(r, sizeof(u32));
    for (u32 i = 0; i < r; i++) {
        PermC[i] = i;
    }

    // debug {
    printf("ordenado\n");
    for (u32 i = 0; i < r; i++)
        printf("%d,", PermC[i]);
    printf("\n");
    // debug }

    srand(R);
    for (u32 i = 0; i < r; i++) {
        // intercambiar el actual con un elemento del índice aleatorio
        u32 indiceAleatorio = rand32() % r;
        u32 temporal = PermC[i];
        PermC[i] = PermC[indiceAleatorio];
        PermC[indiceAleatorio] = temporal;
    }

    // debug {
    printf("permutado\n");
    for (u32 i = 0; i < r; i++)
        printf("%d,", PermC[i]);
    printf("\n");
    // debug }

    for (u32 i = 0; i < n; i++) {
        ColoreoNuevo[i] = PermC[Coloreo[i]];
    }

    free(PermC);
    return ColoreoNuevo;
}

u32 indiceMayor(u32 n, u32* array) {
    u32 mayorIndice = 0;
    for (u32 i = 1; i < n; i++) {
        if (array[i] > array[mayorIndice]) {
            mayorIndice = i;
        }
    }
    return mayorIndice;
}

void printarray(u32 n, u32* array) {  // función innecesaria
    printf("[");
    if (0 < n) {
        printf("%d", array[0]);
        for (u32 i = 1; i < n; i++) {
            printf(", %d", array[i]);
        }
    }
    printf("]\n");
}

u32* RecoloreoCardinalidadDecrecienteBC(u32 n, u32* Coloreo) {
    u32* ColoreoNuevo = calloc(n, sizeof(u32));
    if (ColoreoNuevo == NULL) {
        return NULL;
    }

    // Calculamos cuantos colores hay
    u32 cantColores = cantColoresEnColoreo(Coloreo, n);
    // Contamos cuantos vertices hay de cada color
    u32* contadorColores = calloc(cantColores, sizeof(u32));
    for (u32 i = 0; i < n; i++) {
        contadorColores[Coloreo[i]]++;
    }

    // debug {
    printf("contadorColores:\n");
    printarray(cantColores, contadorColores);
    // debug }

    // Mapeamos los colores que deben ser permutados
    u32* mapeo = calloc(cantColores, sizeof(u32));
    u32 mayorIndice = 0;
    for (u32 i = 0; i < cantColores; i++) {
        mayorIndice = indiceMayor(cantColores, contadorColores);
        // como todo color aparece al menos una vez,
        // podemos asumir que 0 es el neutro del máximo
        contadorColores[mayorIndice] = 0;
        mapeo[i] = mayorIndice;
    }

    // debug {
    printf("mapeo:\n");
    printarray(cantColores, mapeo);
    // debug }

    // Reemplazamos el coloreo
    for (u32 i = 0; i < n; i++) {
        Coloreo[i] = mapeo[Coloreo[i]];
    }

    free(contadorColores);
    free(mapeo);
    return ColoreoNuevo;
}
