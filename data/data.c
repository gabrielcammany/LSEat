
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */

//Includes propios
#include <signal.h>
#include "../lib/includes/socketUtils.h"
#include "../data/include/dataController.h"

//Constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"

#define MSG_RECIEVED_CONNECTION "[Connexion recibida]\n"
#define MSG_PROCESSED_CONNECTION "[Connexion procesada]\n"
#define MSG_DISCONNECT "El client procede adesconectarse\n"
#define EXE "Executant Data\n"

#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"

int sockfd;

typedef struct{
    char type;
    char data[63];
}Trama;



void emptyMemory(){

}

/*
void gestionaTrama(Trama *trama){
    int aux;
    char cadena[10];
    switch ((*trama).type) {
        case 1:

            break;

    }
}
*/

void* connectionHandler(void* arg){

    int socket = *(int*)arg;
    char *type = NULL, *header = NULL, *length = NULL, *data = NULL;
    char trama[155];

    while(1){

        read(socket,&trama, sizeof(trama));
        printf("%s\n", trama);
        /*read(socket,&type, sizeof(char));
        printf("%s\n",type);

        read(socket,&header, sizeof(char) * 10);
        printf("%s\n",header);

        read(socket,&length, sizeof(char) * 2);
        printf("%s\n",length);
*/
        //write(1, MSG_RECIEVED_CONNECTION,strlen(MSG_RECIEVED_CONNECTION));
        //gestionaTrama(&trama);
       // write(socket,&trama, sizeof(trama));
      //  write(1, MSG_PROCESSED_CONNECTION, strlen(MSG_PROCESSED_CONNECTION));

    }

    write(1,MSG_DISCONNECT,strlen(MSG_DISCONNECT));

    close (socket);

    write(1, MSG_PROCESSED_CONNECTION, strlen(MSG_PROCESSED_CONNECTION));
    return NULL;
}


int main (int argc, char *argv[])
{
    int exit = EXIT_SUCCESS;
    sockfd = 0;
    Data data;

    //signal(SIGINT, signalHandler);

    if (argc < 1)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        exit = EXIT_FAILURE;
    }

    if(readDataConfig(argv[1],&data) < 0){
        write(1,ERR_FILE,strlen(ERR_FILE));
        exit = EXIT_FAILURE;
    }

    write(1, EXE, strlen(EXE));

    if(exit != EXIT_FAILURE){
        exit = serialHandler_Data(data.config.picardPort,data.config.IP,connectionHandler);
    }
    emptyMemory();

    return exit;
}