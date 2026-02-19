#ifndef FICHEROS_BASICO_H
#define FICHEROS_BASICO_H

#include "bloques.h"
#include <time.h>
#include <limits.h>

#define posSB 0
#define tamSB 1
#define INODOSIZE 128

struct superbloque {
    unsigned int posPrimerBloqueMB;
    unsigned int posUltimoBloqueMB;
    unsigned int posPrimerBloqueAI;
    unsigned int posUltimoBloqueAI;
    unsigned int posPrimerBloqueDatos;
    unsigned int posUltimoBloqueDatos;
    unsigned int posInodoRaiz;
    unsigned int posPrimerInodoLibre;
    unsigned int cantBloquesLibres;
    unsigned int cantInodosLibres;
    unsigned int totBloques;
    unsigned int totInodos;
    char padding[BLOCKSIZE - 12 * sizeof(unsigned int)];
};

struct inodo {
    unsigned char tipo;     // 'l':libre, 'd':dir, 'f':fich
    unsigned char permisos; // rwx
    unsigned char reservado_alineacion1[6];
    time_t atime; 
    time_t mtime; 
    time_t ctime; 
    time_t btime; 
    unsigned int nlinks;
    unsigned int tamEnBytesLog;
    unsigned int numBloquesOcupados;
    unsigned int punterosDirectos[12];
    unsigned int punterosIndirectos[3];
    char padding[INODOSIZE - 2 * sizeof(unsigned char) - 4 * sizeof(time_t) - 18 * sizeof(unsigned int) - 6 * sizeof(unsigned char)];
};

// Prototipos
int tamMB(unsigned int nbloques);
int tamAI(unsigned int ninodos);
int initSB(unsigned int nbloques, unsigned int ninodos);
int initMB();
int initAI();

#endif
