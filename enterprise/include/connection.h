//
// Created by gabriel on 19/11/17.
//

#ifndef LSEAT_CONNECTION_H
#define LSEAT_CONNECTION_H

#include "controller.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define ERR_CONNECTION "Hi ha hagut un problema de connexio amb Data!\n"


void connection_executeEnterpriseClient();

void connection_createConnectionPicards();

void sendInfoData();

void *connection_Picard();

#endif //LSEAT_CONNECTION_H
