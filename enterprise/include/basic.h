//
// Created by gabriel on 19/11/17.
//

#ifndef LSEAT_BASIC_H
#define LSEAT_BASIC_H

//includes propios
#include "../../lib/include/files.h"

//System includes
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int seconds;
    char *name;
} Restaurant;

typedef struct {
    int unities;
    int price;
    char *name;
} Dish;

typedef struct {
    char *ip;
    int port;
} Data;

typedef struct {
    char *ip;
    int port;
} Picard;

typedef struct {
    Restaurant restaurant;
    Dish *menu;
    Data data;
    Picard picard;
    int num_menu;
} Enterprise;

Enterprise enterprise;

/**
 * Constants for ERRORS
 */

#define ERR_IP "ERROR en llegir la configuració IP\n"
#define ERR_PORT "ERROR en llegir la configuració del port\n"
#define ERR_ENTNAME "ERROR en llegir el nom de l'enterprise\n"
#define ERR_SEC "ERROR en llegir la configuració IP\n"

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void basic_freeMemory();

/**
 * Reads configuration file for enterprise
 * @param fitxer string file name
 * @return returns if everything is ok
 */

int basic_readConfigEnterprise(char *fitxer, char *menu, Enterprise *enterprise);

/**
 * Function that initializes all variables
 * @param enterprise structure with all information
 */
void basic_startValues(Enterprise *enterprise);

    #endif //LSEAT_BASIC_H
