/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include <signal.h>
#include "../include/connection.h"

void *connection_handlerEnterprise(void *arg) {
    Packet packet, response;
    int socket = *((int *) arg);
    packet = extractIncomingFrame(socket);

    if (packet.type == '1' && (strcmp(packet.header, HEADER_DATPIC) == 0) && packet.length > 0) {
        enterprise = extractInfoEnterprise(packet);
        sendConnexionOKPacket(socket,CONNECT);
        close(socket);
        //response = createPacket(CONNECT, HEADER_CON, (unsigned short) 0, "\0");
    } else {
        sendConnexionKOPacket(socket,CONNECT);
       // response = createPacket(CONNECT, HEADER_NCON, (unsigned short) 0, "\0");
    }

   // sendSerialized(socket, response);

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
        sendConnexionKOPacket(socket,CONNECT);
        return NULL;
    }

    //Because is type 1 we know that data contains the client name
    write(1, CONNECTING, strlen(CONNECTING));
    write(1, packet.data, packet.length);
    write(1, "\n\0", sizeof(char) * 2);

    //we check that struct enterprise isnt empty
    if(enterprise.name == NULL || enterprise.ip == NULL || enterprise.ip == 0){
        sendConnexionKOPacket(socket,CONNECT);
        write(1, DISCONNECTING, strlen(DISCONNECTING));
        write(1, packet.data, packet.length);
        write(1, "\n\0", sizeof(char) * 2);
    }else {
        //We create the buffer that will be the data in response to the client
        sprintf(convert, "%d", enterprise.port);
        buffer = createBuffer(3, enterprise.name, convert, enterprise.ip);

        //In case we are not able to create the buffer we close everything
        if (buffer == NULL) {
            sendConnexionKOPacket(socket,CONNECT);
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
            sendConnexionKOPacket(socket,CONNECT);
            return NULL;
        }
        //Once we know if the connexion has been done then we close the file descriptor
        close(socket);
        free(buffer);
    }

    return NULL;
}

void *connection_clientListener(void *socket) {
    write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));
    serialHandler(socketPic, connection_handlerClient);
    return socket;
}

void dNetwork_executeData(int portE, int portP, char *ip) {

     if ((socketPic = createConnectionServer(portP, ip)) > 0) {
         pthread_t thread_id;
         if (pthread_create(&thread_id, NULL, connection_clientListener, NULL) < 0) {
             perror("could not create thread");
             close(socketPic);
         }

     }

    //We listen enterprise port
    if ((socketEnt = createConnectionServer(portE, ip)) > 0) {
        serialHandler(socketEnt, connection_handlerEnterprise);
    }
}



Enterprise extractInfoEnterprise(Packet packet) {
    char *port;
    int i = 0, j = 0;
    Enterprise enterprise;

    enterprise.name = (char *) malloc(sizeof(char));

    while (packet.data[i] != '&' && i < packet.length) {
        enterprise.name[i] = packet.data[i];
        i++;
        enterprise.name = (char *) realloc(enterprise.name, sizeof(char) * (i + 1));
    }
    i++;
    port = (char *) malloc(sizeof(char));

    while (packet.data[i] != '&' && i < packet.length) {
        port[j] = packet.data[i];
        i++;j++;
        port = (char *) realloc(port, sizeof(char) * (j + 1));
    }

    enterprise.port = atoi(port);
    i++;
    j=0;
    enterprise.ip = (char *) malloc(sizeof(char));
    while (packet.data[i] != '&' && i < packet.length) {
        enterprise.ip[j] = packet.data[i];
        i++;j++;
        enterprise.ip = (char *) realloc(enterprise.ip, sizeof(char) * (j + 1));
    }

    return enterprise;
}