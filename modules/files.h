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


//includes propios
#include "types.h"
#include "utils.h"

//constantes propias
#define ERR_ARGUMENT "Format d'obertura no suportat\n"
#define ERR_EMPTY_FILE "El fitxer de configuracio esta buit!\n"
#define ERR_OP_FILE "Error en l'obertura del fitxer!\n"
#define ERR_SALDO_FILE "No s'ha trobat el saldo en el fitxer!\n"
#define ERR_FORMAT_SALDO_FILE "Error en el format del saldo introduit!\n"
#define ERR_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_FORMAT_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_IP_FILE "No s'ha trobat la IP en el fitxer!\n"
#define ERROR_MEMORY "ERROR! No s'ha pogut demanar memoria per desar les dades\n"

int readClientConfig(char *name, ClientLSEat *lseat);

int readNetworkConfig(int socket, Config *config);

/**
 * Function that opens a file
 * @param name Name of file
 * @param status In case of beign 1, O_RDONLY, 2 O_RDWR and 3 O_RDWR and creation of file with 0777
 * @return File descriptor
 */
int openFile(char *name, int status);

int moveToStart(int fd);

int checkEmpty(int fd);

#endif
