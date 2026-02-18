int main(int argc, char **argv){
  if (argc != 3) {
        fprintf(stderr, "Uso: %s <nombre_dispositivo> <nbloques>\n", argv[0]);
        return FALLO;
    }

    const char *nombre = argv[1];
    int nbloques = atoi(argv[2]);

    if (nbloques <= 0) {
        fprintf(stderr, "Número de bloques inválido\n");
        return FALLO;
    }

    // Hay que montar el dispositivo primero
    if (bmount(nombre) == FALLO) {
        return FALLO;
    }

    // inicializar el buffer de un bloque a 0
    unsigned char buffer[BLOCKSIZE];
    memset(buffer, 0, BLOCKSIZE);

    for (int i = 0; i < nbloques; i++) {
        if (bwrite(i, buffer) == FALLO) {
            bumount();
            return FALLO;
        }
    }

    // Desmontarlo para acabar
    if (bumount() == FALLO) {
        return FALLO;
    }

    return EXITO;
}
