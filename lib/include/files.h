/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: fitxers.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */
#ifndef _FILES_H_
#define _FILES_H_

//include propios
#include "utils.h"

//constantes propias
#define ERR_SALDO_FILE "No s'ha trobat el saldo en el fitxer!\n"
#define ERR_FORMAT_SALDO_FILE "Error en el format del saldo introduit!\n"
#define ERR_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_FORMAT_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_IP_FILE "No s'ha trobat la IP en el fitxer!\n"
#define ERROR_MEMORY "ERROR! No s'ha pogut demanar memoria per desar les dades\n"

/**
 * Function that opens a file
 * @param name Name of file
 * @param status In case of beign 1, O_RDONLY, 2 O_RDWR and 3 O_RDWR and creation of file with 0777
 * @return File descriptor
 */
int FILES_openFile(char *name, int status);

/**
 * Function to move pointer of file to start
 * @param fd file descriptor of file
 * @return if everything goes right
 */
int FILES_moveToStart(int fd);

/**
 * Checks if the file is empty
 * @param fd File file descriptor
 * @return if everything goes right
 */
int FILES_checkEmpty(int fd);

#endif
