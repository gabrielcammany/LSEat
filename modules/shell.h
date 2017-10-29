/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef _SHELL_H_
#define _SHELL_H_

//includes del sistema
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
//Includes propios
#include "types.h"
#include "utils.h"

//Constantes propias
#define INTRODUCTION "Introdueixi comandes...\n"
#define COMMANDA_OK "[Comanda OK]\n"
#define COMMANDA_KO "Comanda no reconeguda\n"
#define NO_ARGS "Comanda reconeguda, pero falten arguments!\n"

#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"


#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"

/**
 * Procediment que s'ocupa de la shell personalitzada i de les inputs de l'usuari
 * @param *lseat [description]
 */
void startShell(LSEat *lseat);

/**
 * Funció encarregada de gestionar les commandes per part de l'usuari
 * @param input commanda que s'ha introduit
 * @return
 */
int manageShell(char *input);

/**
 * Procediment per gestionar les comandes amb arguments
 * @param input
 * @return
 */
int specialCommand(char *input);

/**
 * Procediment que comprova si la comanda amb arguments es correcta
 * @param input
 * @param index
 * @return
 */
int checkSpecialCommand(char *input);

#endif
