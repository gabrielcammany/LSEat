/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/controller.h"
#include "../include/connection.h"
#include "../../lib/include/llista.h"

void dControl_signalHandler(int signum) {
    char convert[10];

    switch (signum) {
        case SIGINT:
            close(socketData);
            close(socketPic);
            LLISTA_destrueix(&picards);
            basic_freeMemory(&enterprise);

            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);
            break;
        case SIGALRM:
            printf("SEGONS: %d\n",enterprise.restaurant.seconds);
            write(1,"Han pasat ",strlen("Han pasat "));
            sprintf(convert,"%d",enterprise.restaurant.seconds);
            write(1,convert, sizeof(char));
            write(1," segons\n", strlen(" segons\n"));
            alarm(enterprise.restaurant.seconds);
            sendInfoData();
            break;
        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}


int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise) {
    return basic_readConfigEnterprise(fitxer,menu, enterprise);
}

int controller_eliminaPicard(char *name){
    Element aux;
    int eliminat = 0;

    LLISTA_vesInici(&picards);
    while(!LLISTA_fi(picards)){
        aux = LLISTA_consulta(&picards);
        if(strcmp(aux.name,name) == 0){
            LLISTA_esborra(&picards);
            eliminat = 1;
        }else{
            LLISTA_avanca(&picards);
        }
    }

    return eliminat;
}