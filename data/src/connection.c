//
// Created by gabriel on 19/11/17.
//

#include <signal.h>
#include "../include/connection.h"

void addEnterprise() {

    enterprise = (Enterprise *) malloc(sizeof(Enterprise));
    if (enterprise == NULL) {
        exit(EXIT_FAILURE);
    }

    enterprise->port = 8063;
    enterprise->ip = "127.0.1.1";
    enterprise->numberClients = 0;
    enterprise->name = "Prova";

}


void *connection_handlerEnterprise(void *arg) {
    Packet packet;
    int socket = *((int*)arg);
    packet = extractIncomingFrame(socket);
    printf("Packet type: %c\n",packet.type);
    printf("Packet header: %s\n",packet.header);
    printf("Packet length: %d\n",packet.length);
    printf("Packet data: %s\n",packet.data);
    close(socket);
    return NULL;
}

void *connection_handlerClient(void *arg) {

    int socket = *((int *) arg);
    Packet packet, aux;
    char convert[10], *buffer;

    //We extract the information of the clients packet
    packet = extractIncomingFrame(socket);

    //We know there will be only the type 1 of packet from clients
    if (packet.type != '1' || strcmp(packet.header, HEADER_PICINF) == 0) {
        sendConnexionKOPacket(socket);
        return NULL;
    }

    //Because is type 1 we know that data contains the client name
    write(1, CONNECTING, strlen(CONNECTING));
    write(1, packet.data, packet.length);
    write(1, "\n\0", sizeof(char) * 2);

    //We create the buffer that will be the data in response to the client
    sprintf(convert, "%d", enterprise->port);
    buffer = createBuffer(3, enterprise->name, convert, enterprise->ip);

    //In case we are not able to create the buffer we close everything
    if (buffer == NULL) {
        sendConnexionKOPacket(socket);
        return NULL;
    }

    //we create packet to response client
    aux = createPacket(CONNECT, HEADER_DATPIC, (unsigned short) strlen(buffer), buffer);


    if (aux.type > 0) {
        //if everything is ok then we send information of the enterprise back to the client
        sendSerialized(socket, aux);
        write(1, DISCONNECTING, strlen(DISCONNECTING));
        write(1, packet.data, packet.length);
        write(1, "\n\0", sizeof(char) * 2);

    } else {
        sendConnexionKOPacket(socket);
        return NULL;
    }
    //Once we know if the connexion has been done then we close the file descriptor
    close(socket);
    free(buffer);
    return NULL;
}

void *connection_clientListener(void *socket) {
    write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));
    serialHandler(socketPic, connection_handlerClient);
    return socket;
}

void dNetwork_executeData(int portE, int portP, char *ip) {

   /* if ((socketPic = createConnectionServer(portP, ip)) > 0) {
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_clientListener, NULL) < 0) {
            perror("could not create thread");
            close(socketPic);
        }

    }*/


    //We listen enterprise port
    if((socketEnt = createConnectionServer(portE, ip)) > 0) {
        serialHandler(socketEnt, connection_handlerEnterprise);
    }

    while (1) {}

    //kill(getpid(),SIGUSR1);
}

void sendConnexionKOPacket(int socket) {
    Packet aux;
    //if not we send the KO packet to the client
    aux = createPacket(CONNECT, HEADER_NCON, (unsigned short) 0,"\0");
    sendSerialized(socket, aux);
    write(1, ERR_CLIENT, strlen(ERR_CLIENT));
    write(1, '\0', sizeof(char));
    close(socket);

}