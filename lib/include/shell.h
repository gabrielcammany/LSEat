/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.h
 * @Last modified by:   Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef _SHELL_H_
#define _SHELL_H_

//includes del sistema
#include <termios.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//Includes propios
#include "utils.h"

//Own Constants
#define ADALT "\033[1A"
#define ABAIX "\033[1B"
#define DRETA "\033[1C"
#define ESQUERRA "\033[1D"
#define NETEJAR_LINIA "\033[2K"

#define BUFFER 128
#define BATCH_HISTORY 32


typedef struct {
	char **cmdHistory;
	char **cmdSession;
	int length;
	int lengthSession;
	int historyfd;
} History;


/**
 * Establece el modo de input
 */
int SHELL_setInputMode();

/**
 * Resetea el input que hemos modificado
 */
void SHELL_resetInput();

/**
 * Funcion que se encarga de gestionar el input del usuario
 * @param buffer Estructura de datos donde se vuelca los datos introducidos
 * @param menu Cadena de caracters que incluye el prompt
 */
void SHELL_readInput(char *buffer, char *menu);

/**
 * Funcion que carga la siguiente comanda
 * @return
 */
int SHELL_loadNextCommand();

int SHELL_loadBatch();

void SHELL_freeAndClose();

/**
 * Saves to file the command
 */
void SHELL_saveToFile();

/**
 * Initializes the history of commands
 * @param fd
 */
void SHELL_initializeHistory(int fd);

#endif
