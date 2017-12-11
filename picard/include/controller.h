/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef LSEAT_CONTROLLER_H
#define LSEAT_CONTROLLER_H

//own includes
#include "interface.h"
#include "connection.h"

//Constants to inform of an error happening
#define ERR_ARG "Error en el nombre d'arguments!\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"
#define ERR_NOCONNECTION "No estas connectat a cap Enterprise!\n"
#define ERR_OP "La operació no s'ha podut dur a terme.\n"


//constants to inform user
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define INTRODUCTION "Introdueixi comandes...\n"
#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"

/**
 * Typo to save the information of the Picard
 * Saldo: money left
 * nom: Picard's name
 */
typedef struct {
	int saldo;
	char *nom;
} Client;

/**
 * Typo for the Data configuration
 * IP: Address where we are going to connect to talk to Data
 * Port: interface where Data will be listening
 */
typedef struct {
	char *IP;
	int Port;
} Config;

/**
 * General typo that contains the 2 typos above
 */
typedef struct {
	Config config;
	Client client;
} ClientLSEat;

//variable
ClientLSEat lseat;


/**
 * Function that handles different interruptions
 * @param signum Tells us which interruption
 */
void CONTROLLER_signalHandler(int signum);

/**
 * Function that switches between command
 * and makes what the command is suposed to do
 * @param command Struct with id and data
 * @param lseat general struct
 * @return returns 0
 */
int CONTROLLER_executeCommand(Command command, ClientLSEat lseat);

/**
 * Reads the information of the client configuration
 * @param name Client's name
 * @param lseat general structure
 * @return 0
 */
int BASIC_readClientConfig(char *name, ClientLSEat *lseat);

/**
 * Shows start up message
 */
void BASIC_startupMessages();

/**
 * Function to initialize all variables from command
 * @param command structure that contains command data
 */
void BASIC_startValues(Command *command);

/**
 * Frees all necessary memory
 */
void BASIC_freeMemory();


#endif //LSEAT_CONTROLLER_H
