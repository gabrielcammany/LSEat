
#ifndef basicUtils_H
#define basicUtils_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define MENU "Introdueix el menu en les constants!\n"
#define ERR_OPCIO "\nLa opció que has introduit, no es correcte!\n"
#define ERR_FILE "\nS'ha produit un error al obrir el fitxer!\n"
#define ERR_INT "Interrupció desconeguda!\n"

int readDynamic(char** input, int fd);

int menu();

int writeAndCreateFile(char* input);

//void signalHandler(int signum);

#endif
