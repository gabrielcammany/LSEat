/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shellManager.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef LSEAT_INTERFACE_H
#define LSEAT_INTERFACE_H

//include del sistema
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

//Includes propios
#include "../../lib/include/utils.h"
#include "../../lib/include/shell.h"
#include "../../lib/include/files.h"
#include "../../lib/include/network.h"

//Constantes propias
#define COMMANDA_OK "[Comanda OK]\n"
#define COMMANDA_KO "Comanda no reconeguda\n"

#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"

#define CMD_CONNECTA    1
#define CMD_MENU        2
#define CMD_DEMANA      4
#define CMD_PAGAR       3
#define CMD_DISCONNECT  0

typedef struct {
	char id;
	char **data;
} Command;


void interface_loadHistory();

void interface_saveHistory();

/**
 * Reads commands form user
 * @param cadena user name
 * @return command
 */
Command interface_readCommand(char *cadena);

#endif //LSEAT_SHELLMANAGER_H