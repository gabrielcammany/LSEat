/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shellManager.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef LSEAT_SHELLMANAGER_H
#define LSEAT_SHELLMANAGER_H

//includes del sistema
#include <ctype.h>
//Includes propios
#include "../../lib/includes/utils.h"
#include "../../lib/includes/shell.h"
#include "../../lib/includes/files.h"

//Constantes propias
#define INTRODUCTION "Introdueixi comandes...\n"
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


void loadHistory();

void saveHistory();

Command readCommands(char *cadena);

#endif //LSEAT_SHELLMANAGER_H