#include "basicUtils.h"

/**
Function that reads buffer specified in variable fd, and loads the data to
the input char*
**/
int readDynamic(char** input, int fd) {
    char* temporal = NULL;
    char buffer = ' ';
    int indice = 0;

    while (read (fd, &buffer, sizeof(char)) > 0 && buffer != '\n') {
        temporal = (char*)realloc(*input,sizeof(char)*(indice+1));

        //Utilizamos los punteros auxiliares en todos los casos para evitar
        //problemas de memory leak. Ya que si no se puede pedir mas memoria, el bloque
        //que ya existia, se quedaria sin hacer el free porque perderiamos el puntero
        if (temporal == NULL) {

            if(*input != NULL){
                free(*input);
            }
            return -1;

        }

        *input = temporal;
        (*input)[indice] = buffer;

        indice++;
    }

    temporal = (char*)realloc(*input,sizeof(char)*(indice+1));

    if(temporal == NULL){

        free(*input);
        return -1;

    }

    *input = temporal;
    (*input)[indice] = '\0';

    return indice;
}

int writeAndCreateFile(char* input) {

    int fd = 0;

    fd=open("./mensaje.siglang", O_RDWR | O_APPEND | O_CREAT, 0777);

    if (fd<0){
        write(1,ERR_FILE,strlen(ERR_FILE));
        return -1;
    }

    write(fd,input,strlen(input));

    close(fd);
    return 0;
}

int menu(){

    char buffer = ' ', *opcio = NULL;

    int nOpcio = 0, indice = 0;

    while (1) {

        write (1,MENU,sizeof(char)*strlen(MENU));

        while (read(0,&buffer,sizeof(char)) > 0 && buffer != '\n') {

            opcio = (char*) realloc (opcio, sizeof(char)*(indice+1));
            opcio[indice] = buffer;
            indice++;

        }
        if (indice > 0) {

            opcio = (char*) realloc (opcio, sizeof(char)*(indice+1));
            opcio[indice] = '\0';
            nOpcio = atoi(opcio);

        }
        if (nOpcio < 1 || nOpcio > 4) {

            write (1, ERR_OPCIO, sizeof(ERR_OPCIO));
            indice = 0;

        }else{

            break;

        }
    }

  free(opcio);
  return nOpcio;

}

/*
void signalHandler(int signum) {
    switch (signum) {
		case SIGINT:
		break;
        default:
            write (1, ERR_INT, strlen(ERR_INT));
        break;
    }
}*/
