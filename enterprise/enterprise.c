/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
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

    picards = LLISTA_crea();

    connection_createConnectionPicards();

    return EXIT_SUCCESS;
}