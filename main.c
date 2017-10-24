/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: main.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 25-10-2017
 */

//includes del sistema
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Includes propios
#include "types.h"
#include "fitxers.h"
#include "shell.h"
//Constantes
#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"


int main (int argc,char **argv) {

    LSEat lseat;
    char cadena[100];

    if ( argc != 2 ) {
        write (1, ERR_ARG, strlen(ERR_ARG));
        exit (EXIT_FAILURE);
    }

    lseat = LecturaFitxerConfigClient(argv[1]);

    sprintf(cadena, WELCOME, lseat.client.nom);
    write (1, cadena, strlen(cadena));

    sprintf (cadena, SALDO, lseat.client.saldo);
    write (1, cadena, strlen(cadena));

    Shell(lseat);
    /*printf("NOM: %s\n", lseat.client.nom);
    printf("Saldo: %d\n", lseat.client.saldo);
    printf("IP: %s\n", lseat.config.IP);
    printf("Port: %d\n", lseat.config.Port);*/



    return 0;
}
