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

ClientLSEat lseat;
int socketfd;

int connectToEnterprise(ClientLSEat *lsEat, char *s);

int readClientConfig(char *name, ClientLSEat *lseat);

void signalHandler(int signum);

int readNetworkConfig(int fd, Config *config);

void startupMissages();

void startValues(Command *command);

void freeMemory();

#endif //LSEAT_PICARDCONTROLLER_H
