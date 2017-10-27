
#ifndef socketUtils_H
#define socketUtils_H

#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "basicUtils.h"

#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"
#define MSG_NEW "Nova connexio de %s:%hu\n"
#define MSG_EXIT_CLIENT "Client amb ip %s:%hu s'ha desconnectat\n"
#define MSG_EXIT "\nT'has desconnectat del servidor!\n Fins aviat! \n"
#define MSG_EXIT_SRV "\nEl servidor ha estat desconectat\n"

typedef struct{
    struct sockaddr_in c_addr;
    void* arg;
    int socket;
}ClientInfo;

/**
            - Client functions -
**/

/**
Reads the client configuration from a file.
Returns 1 in case of error.
**/
int readClientConfigFile(char** ip, char** port, char* fileName);

int createConnectionClient(int portInput, char* ipInput);


/**
            - Server functions -
**/

/**
Reads the server configuration from a file.
Returns 1 in case of error.
**/
int readServerConfigFile(char** port, char* fileName);

int createConnectionServer(int portInput);

void serialHandler(int port, void *(*handler) (void *),void *arg, int *sockfd);

void parallelHandler(int port, void *(*handler) (void *),void *arg, int *sockfd);


#endif