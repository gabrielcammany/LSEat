/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: utils.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "types.h"

/**
 * Funcion encargada de leer dinamicamente de un file descriptor
 * @param input Estructura donde se volcara los datos
 * @param fd File descriptor
 * @return Retorna 0 si no ha leido nada, -1 en caso de error de memoria y mayor a 0 cuando la lectura es correcta.
 */
int readDynamic(char **input, int fd);

/**
 * Esta funcion se encarga de extraer partes de una cadena de caracteres delimitados por el delimiter.
 * @param input Cadena a procesar
 * @param delimiter Delimitador
 * @param espais Valor por referencia que contiene los espacios hasta el primer caracter
 * @return Retorna -1 en caso de que el input este vacio sino, el indice hasta el delimitador
 */
int getArrayString(char *input, char delimiter, int *espais);

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void freeMemory(LSEat *lseat);

#endif
