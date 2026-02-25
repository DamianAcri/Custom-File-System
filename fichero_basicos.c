#include bloques.h
#include ficheros_basicos.h

 int tamMB(unsigned int nbloques) {
     return ((nbloques/8)/BLOCKSIZE)+1;
 }  

int tamAI(unsigned int ninodos) {
    return (ninodos * INODOSIZE)/BLOCKSIZE;
}

int initSB(unsigned int nbloques, unsigned int ninodos){
    SB.posPrimerBloqueMB = posSB + tamSB;
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
    SB.posUltimoBloqueDatos = nbloques-1;
    SB.posInodoRaiz = 0;
    SB.posPrimerInodoLibre = 0;
    SB.cantBloquesLibres = nbloques;
    SB.cantInodosLibres = ninodos;
    SB.totBloques = nbloques;
    SB.totinodos = ninodos;
    int numbits= bwrite(posSB, &SB);
    if (numbits == -1) return FALLO;
    return EXITO;

}

int initMB() {
    struct superbloque sb;
    char bufferMB[BLOCKSIZE];

    //leemos sb
    int lect = bread(posSB, &sb); 
    if (lect == -1) return FALLO;

    unsigned int nbloques_metadatos = sb.posPrimerBloqueDatos;
    unsigned int nbloques_MB = tamMB(sb.totBloques);
    unsigned int bitsPorBloque = BLOCKSIZE * 8;
    unsigned int bloquesCompletos = nbloques_metadatos/bitsPorBloque;
    unsigned int restoBits = nbloques_metadatos % bitsPorBloque;
  
    memset(bufferMB, 255, BLOCKSIZE);
    for (unsigned int i = 0; i < bloquesCompletos; i++) {
        if (bwrite(sb.posPrimerBloqueMB + i, bufferMB) == -1) return FALLO;
    }

    if (restoBits > 0 && bloquesCompletos < nbloques_MB) {
        memset(bufferMB, 0, BLOCKSIZE);
        unsigned int bytesCompletos = restoBits / 8;
        unsigned int restoByte = restoBits % 8;

        for (unsigned int j = 0; j < bytesCompletos; j++) {
            bufferMB[j] = 255;
        }
        
        if (restoByte > 0) {
            bufferMB[bytesCompletos] = (255 << (8-restoByte));
        }
        
        if(bwrite(sb.posPrimerBloqueMB + bloquesCompletos, bufferMB) == -1) {
            return FALLO;
        }

        bloquesCompletos++;

    }
    memset (bufferMB, 0, BLOCKSIZE);
    for (unsigned int i = bloquesCompletos; i < nbloques_MB; i++) {
        if (bwrite(sb.posPrimerBloqueMB + i, bufferMB) == -1) {
            return FALLO;
        }
    }

    sb.cantBloquesLibres = nbloques_metadatos;
    if (bwrite(posSB,&sb) == -1) {
        return FALLO;
    }

    return EXITO;
}

int initAI(){
struct inodo inodos [BLOCKSIZE/INODOSIZE];
contInodos := SB.posPrimerInodoLibre + 1;  //si hemos inicializado SB.posPrimerInodoLibre = 0
para (i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) hacer //para cada bloque del AI
    leer el bloque de inodos i  en el dispositivo virtual
    para (j = 0; j < BLOCKSIZE / INODOSIZE;  j++) hacer  //para cada inodo del bloque
       inodos[j].tipo := ‘l’;  //libre
       si (contInodos < SB.totInodos) entonces  //si no hemos llegado al último inodo del AI
               inodos[j].punterosDirectos[0] := contInodos;  //enlazamos con el siguiente
               contInodos++;
        si_no //hemos llegado al último inodo
              inodos[j].punterosDirectos[0] := UINT_MAX;
               //hay que salir del bucle, el último bloque no tiene por qué estar completo !!!
        fsi
   fpara
   escribir el bloque de inodos i  en el dispositivo virtual
fpara

}