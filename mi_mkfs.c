#include "ficheros_basico.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <disco> <nbloques>\n", argv[0]);
        return FALLO;
    }

    const char *nombre_disco = argv[1];
    int nbloques = atoi(argv[2]);

    if (nbloques <= 0) {
        fprintf(stderr, "Número de bloques inválido\n");
        return FALLO;
    }

    // Cálculo del número de inodos
    int ninodos = nbloques / 4;

    // Montar el dispositivo
    if (bmount(nombre_disco) == FALLO) {
        fprintf(stderr, "Error al montar el dispositivo\n");
        return FALLO;
    }

    // Inicializar buffer a 0
    unsigned char buffer[BLOCKSIZE];
    memset(buffer, 0, BLOCKSIZE);

    // Escribir todos los bloques a 0
    for (int i = 0; i < nbloques; i++) {
        if (bwrite(i, buffer) == FALLO) {
            fprintf(stderr, "Error al escribir el bloque %d\n", i);
            bumount();
            return FALLO;
        }
    }

    // Inicializar estructuras del sistema de ficheros
    if (initSB(nbloques, ninodos) == FALLO) {
        fprintf(stderr, "Error al inicializar el superbloque\n");
        bumount();
        return FALLO;
    }

    if (initMB() == FALLO) {
        fprintf(stderr, "Error al inicializar el mapa de bits\n");
        bumount();
        return FALLO;
    }

    if (initAI() == FALLO) {
        fprintf(stderr, "Error al inicializar el array de inodos\n");
        bumount();
        return FALLO;
    }

    // Desmontar el dispositivo
    if (bumount() == FALLO) {
        fprintf(stderr, "Error al desmontar el dispositivo\n");
        return FALLO;
    }

    return EXITO;
}