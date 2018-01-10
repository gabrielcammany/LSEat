/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: connection.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef LSEAT_NETWORK_H
#define LSEAT_NETWORK_H

//system includes
#include <termio.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

//own includes
#include "../../lib/include/network.h"
#include "../../lib/include/menuStructure.h"

//constants to make user know if there is any error
#define ERR_ARG "Error en el nombre d'arguments!\n"
#define ERR_INT "Interrupció desconeguda!\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_CONN "S'ha perdut la connexió!\n"
#define ERR_DATA "Problema amb l'informació rebuda\n"
#define ERR_NUM "Hi ha un error en el numero d'unitats\n"
#define ERR_PLAT "Hi ha un error en el nom del plat\n"

//constants to inform the user
#define SALDO "Té %d euros disponibles\n"
#define BYE "Gràcies per fer servir LsEat. Fins la propera.\n"
#define WELCOME "BENVINGUT %s\n"


//constants from connections of picard
#define CONNECTING "Connectant amb LsEat...\n"
#define CONNECTION_DATA "[Connexió amb Data OK]\n"
#define CONNECTION_NDATA "[Connexió amb Data KO]\n"
#define CONNECTION_ENT "[Connexió amb Enterprise OK]\n"
#define CONNECTION_NENT "[Connexió amb Enterprise KO]\n"
#define DESCONNECTING_OK "[Desconnecta Enterprise OK]\n"
#define DESCONNECTING_KO "[Desconnecta Enterprise KO]\n"

#define NO_PLAT "No s'ha pogut demanar aquest plat!\n"
#define NO_DPLAT "No s'ha pogut eliminar aquest el plat!\n"

#define MENU_SIZE 20
/**
 * Typo to save all information of the enterprise that is connected
 * IP: IP from enterprise where picard will be connected
 * Name: Enterprise Name
 * Port: interface through which we will comunicate with enterprise
 * NumberOfClients: just in case we have to know how many clients
 */
/*
 * GLOBAL VARIABLES
 */

//Enterprise enterprise;
int socketfd; //File descriptor through which we will first communicate to data and then enterprise

/*
 *  OWN FUNCTIONS
 */

/**
 * Function to send information to Data
 * @param socket file descriptor through which we will be sending data
 * @return if everything goes well returns EXIT_SUCCESS, EXIT_FAILURE if not
 */
int CONNECTION_sendInfoData(int socket);

/**
 * Function through which we will make connection with data
 * @param port  interface where we are going to connect
 * @param ip    address where we are going to connect
 * @param name  name of the Picard
 * @return      information that Data gave us
 */
char *CONNECTION_data(int option, int port, char *ip, char *name);

/**
 * Function to connect to enterprise with
 * the information data returned
 * @param data information of Data Server
 * @param nom Picard's name
 * @param saldo money left
 * @return If everything OK the EXIT_SUCCESS, EXIT_FAILURE if not
 */
int CONNECTION_enterprise(char *data, char *nom, int saldo);

/**
 * Function that given a packet, switch the type of the packet
 * and decides what to do
 * @param packet frame's information saved in struct
 * @return if everything is ok returns 0, 1 if not
 */
int CONNECTION_analyseDataPacket(Packet packet);


void CONNECTION_extractEnterpriseData(char *data, char **nom, int *port, char **ip);

/**
 * Function that sends to Enterprise a
 * request to get the menu
 */
void CONNECTION_requestMenuEnterprise();

/**
 * Function that tells the Enterprise to delete a dish.
 * @param data number of dishes and the dish name
 */
void CONNECTION_deleteDishMenu(char **data);

/**
 * Function to tell the Enterprise the Picard wants to pay
 */
void CONNECTION_payEnterprise();

/**
 * Function to tell the Enterprise you want a dish
 * @param data number of dishes and dish name
 */
void CONNECTION_takeNoteEnterprise(char **data);

/**
 * You disconnect from the Enterprise
 * @param nom Picards name who wants to disconnect
 */
void CONNECTION_disconnectEnterprise(char *nom);

/**
 * Function to reconnect an Enterprise
 */
void CONNECTION_enterpriseReconnect();

/**
 * Sends all the commands stored to the new Enterprise
 * @param socket
 * @param table
 */
void CONNECTION_resendCommands(int socket,Menu *table);
#endif
