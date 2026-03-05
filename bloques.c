#include "bloques.h"

// Descriptor global estático (solo visible en este fichero)
static int descriptor = 0;

/**
 * bmount()
 * Abre (o crea) el fichero que simula el dispositivo virtual.
 * Devuelve el descriptor si va bien o FALLO si hay error.
 */
int bmount(const char *camino) {
umask(0111);
    descriptor = open(camino, O_RDWR | O_CREAT, 0666);

    if (descriptor == -1) {
        perror("Error al montar el dispositivo");
        return FALLO;
    }

    return descriptor;
}

/**
 * bumount()
 * Cierra el fichero del dispositivo virtual.
 * Devuelve EXITO o FALLO.
 */
int bumount() {

    if (close(descriptor) == -1) {
        perror("Error al desmontar el dispositivo");
        return FALLO;
    }

    return EXITO;
}

/**
 * bwrite()
 * Escribe un bloque físico (nbloque) desde el buffer.
 * Devuelve número de bytes escritos o FALLO.
 */
int bwrite(unsigned int nbloque, const void *buf) {

    off_t offset = nbloque * BLOCKSIZE;

    if (lseek(descriptor, offset, SEEK_SET) == -1) {
        perror("Error en lseek (bwrite)");
        return FALLO;
    }

    int bytes_escritos = write(descriptor, buf, BLOCKSIZE);

    if (bytes_escritos == -1) {
        perror("Error en write");
        return FALLO;
    }

    return bytes_escritos;
}

/**
 * bread()
 * Lee un bloque físico (nbloque) al buffer.
 * Devuelve número de bytes leídos o FALLO.
 */
int bread(unsigned int nbloque, void *buf) {

    off_t offset = nbloque * BLOCKSIZE;

    if (lseek(descriptor, offset, SEEK_SET) == -1) {
        perror("Error en lseek (bread)");
        return FALLO;
    }

    int bytes_leidos = read(descriptor, buf, BLOCKSIZE);

    if (bytes_leidos == -1) {
        perror("Error en read");
        return FALLO;
    }

    return bytes_leidos;
}
