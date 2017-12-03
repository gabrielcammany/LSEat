
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */

//System includes
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

//Own includes
#include "include/controller.h"
#include "include/connection.h"

int main (int argc, char *argv[])
{
    signal(SIGINT, dControl_signalHandler);
    signal(SIGALRM,dControl_signalHandler);

    if (argc != 3)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        return (EXIT_FAILURE);
    }

    if (controller_readConfiguration(argv[1],argv[2],&enterprise) == EXIT_FAILURE ){
        write(1,ERR_FILE,strlen(ERR_FILE));
        return(EXIT_FAILURE);
    }

    connection_executeEnterpriseClient();

    connection_createConnectionPicards();

    return EXIT_SUCCESS;
}