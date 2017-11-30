//
// Created by gabriel on 19/11/17.
//

#ifndef LSEAT_NETWORK_H
#define LSEAT_NETWORK_H

#include <termio.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../lib/include/network.h"

#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define ERR_INT "Interrupció desconeguda!\n"

#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"

#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_CONN "No s'ha pogut establir connexió!\n"
#define ERR_DATA "Problema amb l'informació rebuda\n"

/**
 * PICARD
 */
#define DISCONNECT_PICENT 2,"[PIC_NAME]"
#define MENU_PICENT 3,"[SHW_MENU]",0,"\0"
#define DISH_PICENT 4,"[NEW_ORD"
#define DELDISH_PICENT 5,"[DEL_ORD]"
#define PAY_PICENT 6,"[PAY]",0,"\0"

#define CONNECTING "Connectant amb LsEat...\n"
#define CONNECTION_DATA "[Connexió amb Data OK]\n"
#define CONNECTION_NDATA "[Connexió amb Data KO]\n"
#define CONNECTION_ENT "[Connexió amb Enterprise OK]\n"
#define CONNECTION_NENT "[Connexió amb Enterprise KO]\n"


/**
 * Function to make the first connection with Data
 * @param socket
 * @param lsEat
 * @return
 */
int connection_sendInfoData(int socket);


/**
 * Function though which we will make connection with data
 * @param lsEat st
 * @param configFile
 * @return
 */

char * connection_data(int port, char *ip, char *name);

/**
 *
 * @param data
 * @param nom
 * @param saldo
 * @return
 */

int connection_enterprise(char* data,char* nom,int saldo);

/**
 *
 * @param packet
 * @return
 */

int analyseDataPacket(Packet packet);

 /**
  *
  * @param socket
  */

void sendConnexionKOPacket(int socket);

/**
 *
 * @param packet
 * @return
 */
int analyseDataPacket(Packet packet);


#endif
