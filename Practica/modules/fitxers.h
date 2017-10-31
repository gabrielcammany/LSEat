/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: fitxers.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */
#ifndef _FITXERS_H_
#define _FITXERS_H_


//inlcludes del sistema
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//includes propios
#include "types.h"
#include "utils.h"
//constantes propias
#define ERR_OP_FILE "Error en l'obertura del fitxer!\n"
#define ERR_EMPTY_FILE "Error el fitxer està buit!\n"
#define ERR_SALDO_FILE "No s'ha trobat el saldo en el fitxer!\n"
#define ERR_FORMAT_SALDO_FILE "Error en el format del saldo introduit!\n"
#define ERR_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_FORMAT_PORT_FILE "No s'ha trobat el port introduit!\n"
#define ERR_IP_FILE "No s'ha trobat la IP en el fitxer!\n"
#define ERROR_MEMORY "ERROR! No s'ha pogut demanar memoria per desar les dades\n"


/**
 * Funcion que se encarga de leer el fichero de configuracion del usuario
 * @param  nombreFichero Recibe el nombre del fichero
 * @return  Retorna la variable con los datos extraidos del fichero
 */
LSEat lecturaFitxerConfigClient(char *nombreFichero);

#endif
