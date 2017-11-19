//
// Created by gabriel on 5/11/17.
//

#ifndef LSEAT_PICARDCONTROLLER_H
#define LSEAT_PICARDCONTROLLER_H


#include "shellManager.h"

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

#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define ERR_INT "Interrupció desconeguda!\n"

#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"

#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_CONN "No s'ha pogut establir connexió!\n"


ClientLSEat lseat;
int socketfd;

/**
 * Function though which we will make connection with data
 * @param lsEat st
 * @param configFile
 * @return
 */
int connectToData(ClientLSEat *lsEat);
/**
 * Reads the information of the client configuration
 * @param name Client's name
 * @param lseat general structure
 * @return
 */
int readClientConfig(char *name, ClientLSEat *lseat);

/**
 * Function that handles different interruptions
 * @param signum Tells us which interruption
 */
void signalHandler(int signum);

/**
 * Reads network configurations
 * @param fd File Descriptor from configuration file
 * @param config Structure where configuration is saved
 * @return
 */
int readNetworkConfig(int fd, Config *config);

void startupMissages();

void startValues(Command *command);

void freeMemory();

void manageCommand(Command command,ClientLSEat lseat);

/**
 * Function to make the first connection with Data
 * @param socket
 * @param lsEat
 * @return
 */
int sendInfoData(int socket,ClientLSEat lsEat);

#endif //LSEAT_PICARDCONTROLLER_H
