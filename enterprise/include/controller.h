//
// Created by gabriel on 5/11/17.
//

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

int socketData;
int socketPic;

/**
 * Function that reads enterprise configuration
 * @param fitxer
 * @param enterprise
 * @return
 */
int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise);

void dControl_signalHandler(int signum);

#endif //LSEAT_ENTERPRISEMANAGER_H
