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

//system includes
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//own constant
#define ERROR_CODE -1

/**
 * Funcion encargada de leer dinamicamente de un file descriptor
 * @param input Estructura donde se volcara los datos
 * @param fd File descriptor
 * @return Retorna 0 si no ha leido nada, -1 en caso de error de memoria y mayor a 0 cuando la lectura es correcta.
 */
int UTILS_readDynamic(char **input, int fd);

/**
 * Esta funcion se encarga de extraer partes de una cadena de caracteres delimitados por el delimiter.
 * @param input Cadena a procesar
 * @param delimiter Delimitador
 * @param espais Valor por referencia que contiene los espacios hasta el primer caracter
 * @return Retorna -1 en caso de que el input este vacio sino, el indice hasta el delimitador
 */
int UTILS_getArrayString(const char *input, char delimiter, int *espais);


/**
 * Function in charge of controling empty inputs
 * @param input users command
 * @return
 */

int UTILS_checkEmptyString(const char *input);

/**
 * Function in charge to lower all characters from string
 * @param input users command
 * @return all string in lower case
 */
char *UTILS_toLower(char *input);

/**
 * With the given arguments creates a buffer
 * @param num number of arguments
 * @param ... arguments
 * @return buffer
 */
char *UTILS_createBuffer(int num, ...);

/**
 * With a given buffer extracts each word delimited by a delimiter
 * @param buffer
 * @param num
 * @param ...
 */
void UTILS_extractFromBuffer(char *buffer, int num, ...);

char *deletePadding(char *input, int max);

void UTILS_debugSTRING(char *string);

#endif
