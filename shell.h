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
#define CLIENT "%s"
#define COMMANDA_OK "[Comanda OK]\n"
#define COMMANDA_KO "Comanda no reconeguda\n"
#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERROR_MEMORY "ERROR! No s'ha pogut demanar memoria\n"
/**
 * Procediment que s'ocupa de la shell personalitzada i de les inputs de l'usuari
 * @param lseat [description]
 */
void Shell(LSEat lseat);

int ManageShell(char *input);

void SpecialCommand(char *input);

void CheckSpecialCommand(char *input, char *string);

#endif
