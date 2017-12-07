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
#include "basic.h"
#include "../../lib/include/network.h"
#include "../../lib/include/utils.h"

/**
 * Error constants
 */

#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"
#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"

//file descriptors to listen different programs
int socketData;
int socketPic;
//list of picards connected to this enterprise
Llista picards;

/**
 * Function that reads enterprise configuration
 * @param fitxer file name with enterprise config
 * @param menu file with all the menu
 * @param enterprise struct to save enterprise information
 * @return
 */
int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise);

/**
 * Function that handles different interruptions
 * @param signum Tells us which interruption
 */
void dControl_signalHandler(int signum);

/**
 * Funtion to eliminate one picard from the list
 * @param name Picard name
 * @return
 */
int controller_eliminaPicard(char *name);

#endif //LSEAT_ENTERPRISEMANAGER_H
