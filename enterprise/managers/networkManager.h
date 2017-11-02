//
// Created by gabriel on 31/10/17.
//

#ifndef LSEAT_CONNECTIONMANAGER_H
#define LSEAT_CONNECTIONMANAGER_H

#define MSG_RECIEVED_CONNECTION "[Connexion recibida]\n"
#define MSG_PROCESSED_CONNECTION "[Connexion procesada]\n"
#define MSG_DISCONNECT "El client procede a desconectarse\n"
#define ERR_CONNECTION "Error en la connexio amb Enterprise!\n"

#include "../../modules/socketUtils.h"
#include "../../modules/files.h"

int connectToEnterprise(ClientLSEat *lsEat, char *s);

#endif //LSEAT_CONNECTIONMANAGER_H
