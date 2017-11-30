
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


int main (int argc, char *argv[])
{
    int i = 0;

    signal(SIGINT, dControl_signalHandler);

    if (argc != 3)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
        exit(EXIT_FAILURE);
    }

    if (controller_readConfiguration(argv[1],argv[2],&enterprise) == EXIT_FAILURE ){
        write(1,ERR_FILE,strlen(ERR_FILE));
        exit(EXIT_FAILURE);
    }

    printf("Enetrprise name: %s\n",enterprise.restaurant.name);
    printf("Segons de refresc: %d\n",enterprise.restaurant.seconds);
    printf("DATA IP: %s\n", enterprise.data.ip);
    printf("DATA PORT: %d\n", enterprise.data.port);
    printf("Picard IP: %s\n", enterprise.picard.ip);
    printf("Picard PORT: %d\n", enterprise.picard.port);
    for( i = 0; i< enterprise.num_menu;i++){
        printf("NOM PLAT: %s\n", enterprise.menu[i].name);
        printf("UNITIES: %d\n", enterprise.menu[i].unities);
        printf("PRICE: %d\n", enterprise.menu[i].price);

    }

    return EXIT_SUCCESS;
}