//
// Created by gabriel on 19/11/17.
//

#ifndef LSEAT_CONTROLLER_H
#define LSEAT_CONTROLLER_H


#include "interface.h"
#include "connection.h"


#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define INTRODUCTION "Introdueixi comandes...\n"

#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"

#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_CONN "No s'ha pogut establir connexió!\n"

typedef struct {
	int saldo;
	char *nom;
} Client;

typedef struct {
	char *IP;
	int Port;
} Config;

typedef struct {
	Config config;
	Client client;
} ClientLSEat;


int socketfd;
ClientLSEat lseat;


/**
 * Function that handles different interruptions
 * @param signum Tells us which interruption
 */
void control_signalHandler(int signum);


void control_executeCommand(Command command, ClientLSEat lseat);

/**
 * Reads the information of the client configuration
 * @param name Client's name
 * @param lseat general structure
 * @return
 */
int basic_readClientConfig(char *name, ClientLSEat *lseat);


void basic_startupMissages();


void basic_startValues(Command *command);


void basic_freeMemory();


#endif //LSEAT_CONTROLLER_H
