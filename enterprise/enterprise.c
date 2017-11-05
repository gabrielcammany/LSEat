
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */

#include <signal.h>
#include "include/enterpriseController.h"

#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"

int sockfd;


void emptyMemory(){

}


int main (int argc, char *argv[])
{
    sockfd = 0;

    //signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        exit(EXIT_FAILURE);
    }


    emptyMemory();

    return EXIT_SUCCESS;
}