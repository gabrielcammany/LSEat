
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */

#include <signal.h>
#include "managers/networkManager.h"

#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"

int sockfd;

typedef struct{
    char type;
    char data[63];
}Trama;



void emptyMemory(){

}

void signalHandler(int signum) {
    switch (signum) {
        case SIGINT:
            close(sockfd);
            emptyMemory();
            exit(0);

        default:
            write (1, ERR_INT, strlen(ERR_INT));
            break;
    }
}

/*
void gestionaTrama(Trama *trama){
    int aux;
    char cadena[10];
    switch ((*trama).type) {
        case 1:

            break;

    }
}*/


void* connectionHandler(void* arg){

    int socket = *(int*)arg;

    Trama trama;
    bzero(trama.data,63);
    trama.type = 0;

    while(trama.type != 8){

        read(socket,&trama, sizeof(trama));

        write(1, MSG_RECIEVED_CONNECTION,strlen(MSG_RECIEVED_CONNECTION));
        //gestionaTrama(&trama);
        write(socket,&trama, sizeof(trama));
        write(1, MSG_PROCESSED_CONNECTION, strlen(MSG_PROCESSED_CONNECTION));

    }

    write(1,MSG_DISCONNECT,strlen(MSG_DISCONNECT));

    close (socket);

    write(1, MSG_PROCESSED_CONNECTION, strlen(MSG_PROCESSED_CONNECTION));
    return NULL;
}


int main (int argc, char *argv[])
{
    char* port = NULL;
    char* ip = NULL;
    int exit = EXIT_SUCCESS;
    sockfd = 0;

    signal(SIGINT, signalHandler);

    if (argc < 3)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        exit = EXIT_FAILURE;
    }

    if(readServerConfigFile(&port ,&ip, argv[1])){
        write(1,ERR_OP_FILE,strlen(ERR_OP_FILE));
        exit = EXIT_FAILURE;
    }

    if(exit != EXIT_FAILURE){
        exit = serialHandler(port,ip,connectionHandler);
    }
    emptyMemory();

    return exit;
}