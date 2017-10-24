/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: fitxers.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 25-10-2017
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

//constantes propias
#define ERR_OP_FILE "Error en l'obertura del fitxer!\n"
#define ERR_EMPTY_FILE "Error el fitxer està buit!\n"
/**
 * Aquesta funció l'utilitzarem per llegir el fitxer de configuració del Client
 * @param  nomFitxer [Reb el nom del fitxer que ha d'obrir]
 * @return           [retorna la configuració i el client]
 */
LSEat LecturaFitxerConfigClient(char nomFitxer[]);

#endif
