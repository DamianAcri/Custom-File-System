#include "ficheros_basico.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: ./leer_sf <disco>\n");
        return -1;
    }

    if (bmount(argv[1]) == FALLO) return -1;

    struct superbloque SB;
    bread(posSB, &SB);

    printf("--- DATOS DEL SUPERBLOQUE ---\n");
    printf("Primer bloque MB: %u\n", SB.posPrimerBloqueMB);
    printf("Ultimo bloque MB: %u\n", SB.posUltimoBloqueMB);
    printf("Primer bloque AI: %u\n", SB.posPrimerBloqueAI);
    printf("Ultimo bloque AI: %u\n", SB.posUltimoBloqueAI);
    printf("Primer bloque Datos: %u\n", SB.posPrimerBloqueDatos);
    printf("Ultimo bloque Datos: %u\n", SB.posUltimoBloqueDatos);
    printf("Posicion inodo raiz: %u\n", SB.posInodoRaiz);
    printf("Posicion primer inodo libre: %u\n", SB.posPrimerInodoLibre);
    printf("Cant. Bloques Libres: %u\n", SB.cantBloquesLibres);
    printf("Cant. Inodos Libres: %u\n", SB.cantInodosLibres);
    printf("Total Bloques: %u\n", SB.totBloques);
    printf("Total Inodos: %u\n", SB.totInodos);
    printf("sizeof struct superbloque: %lu\n", sizeof(struct superbloque));
    printf("sizeof struct inodo: %lu\n", sizeof(struct inodo));

    /* printf("\nRECORRIDO LISTA ENLAZADA DE INODOS LIBRES\n");
    unsigned int pos = SB.posPrimerInodoLibre;
    while (pos != UINT_MAX) {
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    unsigned int bloque = SB.posPrimerBloqueAI + (pos / (BLOCKSIZE / INODOSIZE));
    unsigned int offset = pos % (BLOCKSIZE / INODOSIZE);
    bread(bloque, inodos);
    unsigned int siguiente = inodos[offset].punterosDirectos[0];
    if (siguiente == UINT_MAX)
        printf("-1 ");
    else
        printf("%u ", siguiente);

    pos = siguiente;
    } */

    bumount();
    return 0;
}