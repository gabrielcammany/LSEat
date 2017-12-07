/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#ifndef LSEAT_CONNECTION_H
#define LSEAT_CONNECTION_H

//own includes
#include "controller.h"

//system includes
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

//constant to tell there is an error in the connection
#define ERR_CONNECTION "Hi ha hagut un problema de connexio amb Data!\n"

/**
 * Send info of the Enterprise to Data every X time
 */
void sendInfoData();

/**
 * Funtions to create the thread that sends info to Data every X time
 */
void connection_executeEnterpriseClient();
void *connection_dataListener(void *arg);

/**
 * Funtions to create the thread for every Picard
 * @return
 */
void *connection_Picard();
void connection_createConnectionPicards();

/**
 * Function to analyse the information Picard sends
 * @param socket File descriptor through which Picard talks
 * @param packet Info recieved
 * @return
 */
int analysePacketPicard(int socket, Packet packet);

#endif //LSEAT_CONNECTION_H
