#include "bloques.h"
#define BLOCKSIZE 1024 // bytes
int descriptor;
int bmount(const char *camino){  
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
    
}

 int bread(unsigned int nbloque, void *buf) {
    
}