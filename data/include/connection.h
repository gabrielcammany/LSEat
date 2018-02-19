/**
						SEM_wait(&hashinsert);
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

//system include
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

//own includes
#include "../../lib/include/network.h"
#include "dataStructure.h"

//own Constants
#define BYE "Apagant el servidor Data...\n"
#define NBYE "Error en Data. Apagant el servidor...\n"
#define ERR_UPDATE "Avisant d'error perquè realitzi una reconnexió!\n"

#define CONNECTING "Connectant "
#define DISCONNECTING "Desconnectant "
#define DCONNECT_ERR "Desconnectant per error "
#define DISC_ENT "Desconnectant %s\n"

#define MAX_ENTERPRISES 20

pthread_t thread_id;
int socketPic, socketEnt;
extern pthread_mutex_t mtx;

Table enterprise;
/**
 * Executes all the process of Data for comunication
 * @param portE
 * @param portP
 * @param ip
 */
void CONNECTION_executeData(int portE, int portP, char *ip);

#endif //LSEAT_CONNECTION_H
