#include "bloques.h"
static int descriptor = 0;

int bmount(const char *camino){  
   umask(0000);
   descriptor= open(camino,O_RDWR | O_CREAT,0666);
   if(descriptor == -1){
    perror("Error al abrir el archivo");
    return -1;
   }

   return descriptor;

}

int bumount(){
    if(close(descriptor)==-1){
        perror("Error al hacer el close")
        return -1;
    }

    return 0;
}


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

 int bread(unsigned int nbloque, void *buf) {
    off_t offset = nbloque * BLOCKSIZE;
    if (lseek(descriptor, offset, SEEK_SET) == -1) {
        perror("Error en lseek (bread)");
        return FALLO;
    }

    size_t leidos = read(descriptor, buf, BLOCKSIZE);
    if (bytes_leidos == -1) {
        perror("Error en read");
        return FALLO;
    }

    return bytes_leidos;
}
