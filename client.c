/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

//includes del sistema
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

//Includes propios
#include "types.h"
#include "fitxers.h"
#include "shell.h"
#include "shellManager.h"
//Constantes
#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define ERR_INT "Interrupció desconeguda!\n"


LSEat lseat;

void signalHandler(int signum) {
    switch (signum) {
        case SIGINT:
            free(lseat.client.nom);
            free(lseat.config.IP);
            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));
            resetInput();
            exit(0);
        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}


int main(int argc, char **argv) {
    char cadena[100];

    signal(SIGINT, signalHandler);

    if (argc != 2) {
        write(1, ERR_ARG, strlen(ERR_ARG));
        exit(EXIT_FAILURE);
    }

    lseat = lecturaFitxerConfigClient(argv[1]);

    sprintf(cadena, WELCOME, lseat.client.nom);
    write(1, cadena, strlen(cadena));

    sprintf(cadena, SALDO, lseat.client.saldo);
    write(1, cadena, strlen(cadena));

    startShell(&lseat);


    return 0;
}
