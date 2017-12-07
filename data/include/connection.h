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

//system include
#include <unistd.h>
#include <stdio.h>

//own includes
#include "../../lib/include/network.h"
#include "controller.h"


#define CONNECTING "Connectant "
#define DISCONNECTING "Desconnectant "

/**
 * Structure to save information of Enterprise
 * IP: IP from enterprise
 * port: Port to connect to enterprise
 */
typedef struct{
	char* ip;
	char* name;
	int port;
	char numberClients;
}Enterprise;

Enterprise enterprise;

int socketPic, socketEnt;

/**
 * Extracts info received from Enterprise
 * @param packet struct wir frame's information
 * @return
 */
Enterprise extractInfoEnterprise(Packet packet);

void dNetwork_executeData(int portE, int portP, char* ip);

#endif //LSEAT_CONNECTION_H
