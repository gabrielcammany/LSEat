//
// Created by gabriel on 5/11/17.
//

#ifndef LSEAT_CONTROLLER_H
#define LSEAT_CONTROLLER_H


#include <signal.h>
#include <stdlib.h>

//Includes propios
#include "../../lib/include/utils.h"
#include "../../lib/include/files.h"
#include "../../lib/include/network.h"
#include "connection.h"

//Constantes propias
#define ERR_PORTP "Error en llegir el port del Picard!\n"
#define ERR_IP "Error en llegir la IP de Data\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"

//Constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"

#define EXE "Executant Data\n"

#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"



/**
 * Structure to save data configuration
 * IP: IP of the server data
 * picardPort: Port where picards will connect
 * enterprisePort: Port where enterprise will connect
 */
typedef struct{
    char* ip;
    int picardPort;
    int enterprisePort;
}dConfig;

dConfig data;

/**
 * Function to handle signal
 * @param signum
 */
void dCONTROLLER_signalHandler(int signum);

/**
 * Function which reads the configuration of data
 * @param data Global structure of Data
 * @return if there is an error or not
 */
int BASIC_readDataConfig(char* name,dConfig *data);

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void BASIC_freeMemory(dConfig *data);

/**
 * Funtion to initalize all the variable and structures
 * @param data
 */
void BASIC_startValues(dConfig *data);




#endif //LSEAT_CONTROLLER_H
