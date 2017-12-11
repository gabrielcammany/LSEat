/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef LSEAT_ENTERPRISEMANAGER_H
#define LSEAT_ENTERPRISEMANAGER_H

//Includes del sistema
#include <signal.h>

//Includes propios
#include "../../lib/include/network.h"
#include "../../lib/include/utils.h"
#include "../../lib/include/files.h"
#include "connection.h"
#include "../../lib/include/menuStructure.h"
#include "../include/picardStructure.h"
#include "../../lib/include/menuStructure.h"

/**
 * Error constants
 */

#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"
#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"

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

#define MAX_PICARDS 10
#define MENU_SIZE 20


typedef struct {

	int seconds;
	char *name;
	Menu menu;

} Restaurant;

typedef struct {

	char *data_ip;
	char *data_port;
	char *picard_ip;
	char *picard_port;

} Config;

typedef struct {

	Restaurant restaurant;

	Config config;

	Table clients;

	pthread_t thread_data;

} Enterprise;

Enterprise enterprise;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//file descriptors to listen different programs
int socketData;
int socketPic;
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
int BASIC_readConfigEnterprise(char *fitxer);

/**
 * Function that initializes all variables
 * @param enterprise structure with all information
 */
void BASIC_startValues();

int BASIC_readMenu(char *menu);

void BASIC_welcomeMessage();

/**
 * Function that handles different interruptions
 * @param signum Tells us which interruption
 */
void eCONTROLLER_signalHandler(int signum);

#endif //LSEAT_ENTERPRISEMANAGER_H
