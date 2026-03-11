#include "bloques.h"
#include "ficheros_basico.h"

 int tamMB(unsigned int nbloques) {
     return ((nbloques/8)/BLOCKSIZE)+1;
 }  

int tamAI(unsigned int ninodos) {
    return (ninodos * INODOSIZE)/BLOCKSIZE;
}

int initSB(unsigned int nbloques, unsigned int ninodos){
    struct superbloque SB;
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
    SB.totInodos = ninodos;
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

    sb.cantBloquesLibres = sb.totBloques - nbloques_metadatos;
    if (bwrite(posSB,&sb) == -1) {
        return FALLO;
    }

    return EXITO;
}

int initAI() {

    struct superbloque SB;
    bread(posSB, &SB);

    struct inodo inodos[BLOCKSIZE / INODOSIZE];

    unsigned int contInodos = SB.posPrimerInodoLibre + 1;

    for (int i = SB.posPrimerBloqueAI;
         i <= SB.posUltimoBloqueAI;
         i++) {

        bread(i, inodos);

        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++) {

            inodos[j].tipo = 'l';

            if (contInodos < SB.totInodos) {
                inodos[j].punterosDirectos[0] = contInodos;
                contInodos++;
            } else {
                inodos[j].punterosDirectos[0] = UINT_MAX;
                break;
            }
        }

        bwrite(i, inodos);
    }

    return EXITO;
}
int traducir_bloque_inodo(unsigned int inodo, unsigned int nblogico, unsigned char reservar){
       
    unsigned int  ptr, ptr_ant, salvar_inodo;  
    int nRangoBL, nivel_punteros, indice;
    unsigned int buffer[NPUNTEROS];
    struct inodo inodo;

   ptr,ptr_ant,salvar_inodo,indice,=0;
   
   leer_inodo(ninodo, &inodo) 
   nRangoBL := obtener_nRangoBL(&inodo, nblogico, &ptr); //0:D, 1:I0, 2:I1, 3:I2
   nivel_punteros := nRangoBL //el nivel_punteros +alto es el que cuelga directamente del inodo
   nivel_punteros = nRangoBL

   if(nRangoBL==0){ //Caso punteros Directos

      if(ptr==0){ //no existe bloque de datos
         if(reservar==0) return FALLO;   //error ∄ bloque -> no imprimir error por pantalla!!! 
         ptr = reservar_bloque() //de datos
         inodo.numBloquesOcupados++;
         inodo.ctime = time(NULL); //fecha actual
         inodo.punterosDirectos[nblogico] = ptr; //asignamos la direción del bl. de datos en el inodo
         salvar_inodo = 1;
      


    // Damian
    } 
}else {
        while (nivel_punteros > 0) {
            if (ptr == 0) {
                if (reservar == 0) {
                    return -1; // error ∄ bloque -> no imprimir error por pantalla!!!
                }

                ptr = reservar_bloque(); // de punteros
                inodo.numBloquesOcupados++;
                inodo.ctime = time(NULL);
                salvar_inodo = 1;

                if (numero_punteros == nRangoBL) {
                    inodo.punterosIndirectos[nRangoBL - 1] = ptr;
                } else {
                    buffer[indice] = ptr;
                    bwrite(ptr_ant, buffer); 
                }

                memset(buffer, 0, BLOCKSIZE); // ponemos a 0 todos los punteros del buffer

            } else {
                bread(ptr, buffer);//leemos del dispositivo el bloque de punteros ya existente
            }

            indice = obtener_indice(nblogico, nivel_punteros);
            ptr_ant = ptr;
            ptr = buffer[indice]; // y lo desplazamos al siguiente nivel
            nivel_punteros--;
        }

        if (ptr == 0) {
            if (reservar == 0) {
                return -1; // bloque inexistente -> no imprimir error por pantalla!!!
            }

            ptr = reservar_bloque(); // de datos
            inodo.numBloquesOcupados++;
            inodo.ctime = time(NULL);
            salvar_inodo = 1;
            buffer[indice] = ptr; // asignamos la dirección del bloque de datos en el buffer
            bwrite(ptr_ant, buffer); // salvamos en el dispositivo el buffer de punteros modificado
        }
        
    } 

    if (salvar_inodo) {
        escribir_inodo(ninodo, &inodo);
    }

    return ptr; 
    
}
