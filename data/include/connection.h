//
// Created by gabriel on 19/11/17.
//

#ifndef LSEAT_CONNECTION_H
#define LSEAT_CONNECTION_H

#include <unistd.h>
#include <stdio.h>

#include "../../lib/include/network.h"
#define CONNECTING "Connectant "
#define DISCONNECTING "Desconnectant "
#define ERR_CLIENT "Error en la connexió de "

/**
 * Structure to save information of Enterprise
 * IP: IP from enterprise
 * port: Port to connect to enterprise
 */
typedef struct{
	char* IP;
	int port;
	char* name;
}Enterprise;

Enterprise enterprise;

void* connection_Handler(void* arg);


#endif //LSEAT_CONNECTION_H
