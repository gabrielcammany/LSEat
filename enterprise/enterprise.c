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
#include <stdlib.h>

//Own includes
#include "include/controller.h"

int main (int argc, char *argv[])
{
    signal(SIGINT, eCONTROLLER_signalHandler);
	signal(SIGUSR1,eCONTROLLER_signalHandler);
	signal(SIGUSR2,eCONTROLLER_signalHandler);

    if (argc != 3){
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        exit(EXIT_FAILURE);
    }

	BASIC_startValues();

    if (BASIC_readConfigEnterprise(argv[1]) == ERROR_CODE ){
        write(1,ERR_FILE,strlen(ERR_FILE));
		kill(getpid(),SIGUSR1);
    }

	BASIC_welcomeMessage();

	if (BASIC_readMenu(argv[2]) == ERROR_CODE){
		write(1,ERR_FILE,strlen(ERR_FILE));
		kill(getpid(),SIGUSR1);
	}

    if(CONNECTION_executeEnterpriseClient() < 0){
		kill(getpid(),SIGUSR1);
    }

    CONNECTION_createConnectionPicards();

    return EXIT_SUCCESS;
}