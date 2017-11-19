/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: socketUtils.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef socketUtils_H
#define socketUtils_H

//includes propios
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "utils.h"

//constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"
#define MSG_NEW "Nova connexio de %s:%hu\n"


#define PIC_NAME "0x01"
#define ENT_INF "0x01"
#define CON_KO "0x01"

/**
 * Client Picard
 */
#define CONNECTING "Connectant amb LsEat...\n"
/**
 * Server Data
 */

#define WAIT_CLIENT "Esperant clients...\n"

typedef struct {
    struct sockaddr_in c_addr;
    void *arg;
    int socket;
} ClientInfo;

/**
            - Client functions -
**/

/**
Reads the client configuration from a file.
Returns 1 in case of error.
**/
int readClientConfigFile(char **ip, char **port, char *fileName);

int createConnectionClient(int portInput, char *ipInput);


/**
            - Server functions -
**/

/**
Reads the server configuration from a file.
Returns 1 in case of error.
**/
int readServerConfigFile(char **port, char **ip, char *fileName);

int createConnectionServer(int portInput, char *ipInput);

int serialHandler_Data(char* port, char* ip, void *(*handler)(void*));

int serialHandler_Enterprise(char* port, char* ip, void *(*handler)(void*));

void parallelHandler(char *port, char *ip, void *(*handler)(void *), void *arg);


#endif
