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
#define ERR_PORTE "Error en llegir el port del Enterprise!\n"
#define ERR_IP "Error en llegir la IP de Data\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_FORMAT_PORT "Hi ha un problema amb el format del port!\n"

//Constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"

#define MSG_RECIEVED_CONNECTION "[Connexion recibida]\n"
#define MSG_PROCESSED_CONNECTION "[Connexion procesada]\n"
#define MSG_DISCONNECT "El client procede adesconectarse\n"
#define EXE "Executant Data\n"

#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_ARGS "Error: falta especificar els arxius!\n"
#define BYE "Apagant el servidor Data...\n"


/**
 * Structure to save data configuration
 * IP: IP of the server data
 * picardPort: Port where picards will connect
 * enterprisePort: Port where enterprise will connect
 */
typedef struct{
    char* IP;
    char* picardPort;
    char* enterprisePort;
}Data;


int socketfd;


void control_signalHandler(int signum);


int control_executeData(Data *data);


/**
 * Function which reads the configuration of data
 * @param data Global structure of Data
 * @return if there is an error or not
 */
int basic_readDataConfig(char* name,Data *data);

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void basic_freeMemory(Data *data);


void basic_startValues(Data *data);

#endif //LSEAT_CONTROLLER_H
