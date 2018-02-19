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
#define UPDATE_ERR "Problemes de connexio amb data... Intentant reconnectar! %d de %d Intents.\n"
#define UPDATE_ERR_OK "\nS'ha pogut tornar a establir la connexió, actualització automàtica activa!\n\n"
#define ERR_CONN_PIC "Error en la conexió de %s! Desconectant...\n"
#define ERR_CONN_CLIENT "Error en la conexió d'un client!\n"
#define ERR_LIMIT "Màxim d'intents per reconnectar. Apagant actualització automàtica...\n"
#define UPDATE_ERR_DATA "Data ha retornat error al actualitzar. Apagant actuaització\n"
#define UPDATE_ERR_DATA_REC "Data ha retornat error al actualitzar. Tornant a realitzar nova connexió.\n"
#define UPDATE_ERR_REC_OK "Reconnexió realitzada correctament.\n\n"

#define LIMIT_RECONNECT 10

/**
 * Funtions to create the thread that sends info to Data every X time
 */
int CONNECTION_executeEnterpriseClient();
/**
 *
 * @param arg
 * @return
 */
void *CONNECTION_dataListener(void *arg);

/**
 * Funtions to create the thread for every Picard
 * @return
 */
void *CONNECTION_Picard(void *arg);
void CONNECTION_createConnectionPicards();

/**
 * Function to analyse the information Picard sends
 * @param socket File descriptor through which Picard talks
 * @param packet Info recieved
 * @return
 */
int CONNECTION_analysePacketPicard(int socket, Packet packet);

#endif //LSEAT_CONNECTION_H
