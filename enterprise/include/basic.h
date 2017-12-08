/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: basic.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef LSEAT_BASIC_H
#define LSEAT_BASIC_H

//includes propios
#include "../../lib/include/files.h"

//System includes
#include <stdio.h>
#include <stdlib.h>

typedef struct {

	int units;
	int price;
	char *name;

} Dish;

typedef struct {

	int seconds;
	char *name;
	Dish *menu;
	int num_menu;

} Restaurant;

typedef struct {

	char *name;
	int money;

} Client;

typedef struct {

	char *data_ip;
	char *data_port;
	char *picard_ip;
	char *picard_port;

} Config;

typedef struct {

	Restaurant restaurant;

	Config config;

	Client *clients;
	int num_clients;

} Enterprise;

Enterprise enterprise;

/**
 * Constants for ERRORS
 */

#define ERR_IP "ERROR en llegir la configuració IP\n"
#define ERR_ENTNAME "ERROR en llegir el nom de l'enterprise\n"
#define ERR_SEC "ERROR en llegir la configuració IP\n"
#define ERR_MEMORY "No s'ha pogut alliberar la memoria correctament.\n"


/**
 * General Constants
 */
#define BYE "Tancant restaurant...\n"
#define WELCOME "Benvingut al restaurant "
#define MENU_READY "Carregat Menu!\n"


/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void BASIC_freeMemory();

/**
 * Function that reads enterprise configuration
 * @param fitxer file name with enterprise config
 * @param menu file with all the menu
 * @param enterprise struct to save enterprise information
 * @return
 */
int BASIC_readConfigEnterprise(char *fitxer, Enterprise *enterprise);

/**
 * Function that initializes all variables
 * @param enterprise structure with all information
 */
void BASIC_startValues();

int BASIC_readMenu(char *menu, Enterprise *enterprise);

void BASIC_welcomeMessage(Enterprise enterprise);


#endif //LSEAT_BASIC_H
