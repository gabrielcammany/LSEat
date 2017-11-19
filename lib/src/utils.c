/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: utils.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include <memory.h>
#include <ctype.h>
#include <stdarg.h>
#include "../include/utils.h"


int readDynamic(char **input, int fd) {

    char *temporal = NULL;
    char buffer = ' ';
    int indice = 0;

    while (read(fd, &buffer, sizeof(char)) > 0 && buffer != '\n') {
        temporal = (char *) realloc(*input, sizeof(char) * (indice + 1));

        if (temporal == NULL) {

            if (*input != NULL) {
                free(*input);
            }
            return -1;

        }

        *input = temporal;
        (*input)[indice] = buffer;
        indice++;
    }

    if (*input != NULL) {
        temporal = (char *) realloc(*input, sizeof(char) * (indice + 1));
        if (temporal == NULL) {

            free(*input);
            return -1;

        }

        *input = temporal;
        (*input)[indice] = '\0';
    }

    return indice;

}


int getArrayString(const char *input, char delimiter, int *espais) {

    int index = 0, buida = 0;

    for (*espais = 0; input[*espais] == ' ' || input[*espais] == '\t'; (*espais)++) {}

    for (index = *espais; input[index] != delimiter && input[index] != '\0'; index++) {

        if (input[index] != ' ') {
            buida = 1;
        }
    }

    if (!buida) {
        return -1;
    }

    return index;
}

char* toLower(char *input){
	int i = 0;
	while (input[i]) {
		input[i] = (char) tolower(input[i]);
		i++;
	}
	return input;
}


int checkEmptyString(const char *input){
    int index = 0;

    for (index = 0; index < (int)strlen(input); index++) {

        if (input[index] != ' ') {
            return 0;
        }
    }
    return 1;

}

char* createBuffer(int num, ...){

	char* buffer = NULL, *bufferAux = NULL, *aux = NULL;
	int size, i;

	va_list a_list;
	va_start( a_list, num );

	aux = va_arg ( a_list, char * );
	size = (int)strlen(aux);

	buffer = malloc(sizeof(char)* size);

	if(buffer == NULL){
		return NULL;
	}

	strcat(buffer,aux);

	for (i = 0; i < num; i++) {
		strcat(buffer,"&");

		aux = va_arg ( a_list, char * );
		size = size + (int)strlen(aux);

		bufferAux = realloc(buffer, sizeof(char) * size);

		if(bufferAux == NULL){
			free(buffer);
			return NULL;
		}

		strcat(buffer,aux);
	}
	return buffer;

}