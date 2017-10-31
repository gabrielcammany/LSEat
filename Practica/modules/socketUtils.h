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

#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "utils.h"

#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"
#define MSG_NEW "Nova connexio de %s:%hu\n"

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

int serialHandler(char* port, char* ip, void *(*handler)(void*));

void parallelHandler(char *port, char *ip, void *(*handler)(void *), void *arg);


#endif
