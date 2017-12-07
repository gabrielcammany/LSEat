/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: connection.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/connection.h"

char *connection_data(int port, char *ip, char *name) {
    Packet packet;
    packet.data = NULL;

    //Message to make the user know we are connecting to Data
    write(1, CONNECTING, strlen(CONNECTING));

    //Returns the file descriptor to talk with Data server
    socketfd = createConnectionClient(port, ip);


    if (socketfd < 0) {
        write(1, ERR_CONN, strlen(ERR_CONN));

    } else {
        //We create a packet and fill it with information
        packet = createPacket(CONNECT, HEADER_PICDAT, (unsigned short) strlen(name), name);

        if (packet.type > 0) {
            //then we send the packet serialized
            if (sendSerialized(socketfd, packet) > 0) {
                //we receive the response
                packet = extractIncomingFrame(socketfd);
                //we analyse the information we have recieved
                analyseDataPacket(packet);

            }

        }
    }

    close(socketfd);

    return packet.data;
}


void extractEnterpriseData(char *data, char **nom, int *port, char **ip) {
    char *token, auxPort[5];

    //This function separates the information of the frame by the char &
    //We make this three times because we know it will have 3 fields
    token = strtok(data, "&");

    if (token != NULL) {

        *nom = (char *) malloc(sizeof(char) * strlen(token));

        if (*nom != NULL) {
            strcpy(*nom, token);
        } else {
            write(0, ERR_MEMORY, strlen(ERR_MEMORY));
        }
    }

    token = strtok(NULL, "&");

    if (token != NULL) {
        strcpy(auxPort, token);
        *port = atoi(auxPort);
    } else {
        *port = 0;
    }

    token = strtok(NULL, "&");

    if (token != NULL) {
        *ip = (char *) malloc(sizeof(char) * strlen(token));

        if (*ip != NULL) {
            strcpy(*ip, token);
        } else {
            write(0, ERR_MEMORY, strlen(ERR_MEMORY));
        }
    }
}


int connection_enterprise(char *data, char *nameUser, int saldo) {
    char *ip = NULL, *nameEnterprise = NULL, *userData = NULL, money[10];
    int port;

    //First we extract the information
    //from enterprise that Data sent us
    extractEnterpriseData(data, &nameEnterprise, &port, &ip);

    if (nameEnterprise != NULL && ip != NULL && port > 0) {
        //we create connection between Picard and Enterprise
        socketfd = createConnectionClient(port, ip);

        if (socketfd < 0) {
            write(1, ERR_CONN, strlen(ERR_CONN));
        } else {
            //we create a buffer with all the information
            //of the picard in order to create a packet with it
            //and after serializating it and send it we wait for a response
            sprintf(money, "%d", saldo);
            userData = createBuffer(2, nameUser, money);

            if (userData != NULL) {

                Packet packet = createPacket(CONNECT, HEADER_PICINF, (int) strlen(userData), userData);

                if (packet.type > 0) {

                    sendSerialized(socketfd, packet);

                    if (readSimpleResponse(socketfd) > 0) {

                        write(1, CONNECTION_ENT, strlen(CONNECTION_ENT));

                    } else {
                        //If something goes wrong we will disconnect
                        write(1, CONNECTION_NENT, strlen(CONNECTION_NENT));
                        close(socketfd);
                        socketfd = -1;
                    }

                }

            } else {
                write(0, ERR_MEMORY, strlen(ERR_MEMORY));
            }


        }
    } else {
        write(1, ERR_DATA, strlen(ERR_DATA));
    }

    free(userData);
    return socketfd;
}

int analyseDataPacket(Packet packet) {

    switch (packet.type) {
        case '1':
            if (packet.length == 0) {
                //If there is any error during the connection then we will recive a length of 0
                write(1, CONNECTION_NDATA, strlen(CONNECTION_NDATA));

            } else {
                write(1, CONNECTION_DATA, strlen(CONNECTION_DATA));
            }
            break;
        default:
            break;
    }

    return 0;
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
        i++;
        j++;
        port = (char *) realloc(port, sizeof(char) * (j + 1));
    }

    enterprise.port = atoi(port);
    i++;
    j = 0;
    enterprise.ip = (char *) malloc(sizeof(char));
    while (packet.data[i] != '&' && i < packet.length) {
        enterprise.ip[j] = packet.data[i];
        i++;
        j++;
        enterprise.ip = (char *) realloc(enterprise.ip, sizeof(char) * (j + 1));
    }

    return enterprise;
}

void connection_requestMenuEnterprise() {
    if (socketfd > 2) {
        Packet packet = createPacket(MENU, MENU_PICENT, (unsigned short) 0, "\0");
        sendSerialized(socketfd, packet);
    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }

}

void connection_deleteDishMenu(char **data) {
    if (socketfd > 2) {
        char *buffer = createBuffer(2, data[1], data[0]);
        Packet packet = createPacket(DEL_DISH, DEL_ORD, (unsigned short) strlen(buffer), buffer);
        sendSerialized(socketfd, packet);
    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }
}

void connection_payEnterprise() {
    if (socketfd > 2) {
        Packet packet = createPacket(PAY, PAY_HEADER, (unsigned short) 0, "\0");
        sendSerialized(socketfd, packet);
    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }
}

void connection_takeNoteEnterprise(char **data) {

    if (socketfd > 2) {
        char *buffer = createBuffer(2, data[1], data[0]);
        Packet packet = createPacket(DISH, NEW_ORD, (unsigned short) strlen(buffer), buffer);
        sendSerialized(socketfd, packet);
    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }
}

void connection_disconnectEnterprise(char *nom) {
    if (socketfd > 2) {
        Packet packet = createPacket(DISCONNECT, HEADER_PICDAT, (unsigned short) strlen(nom), nom);
        sendSerialized(socketfd, packet);

        if (readSimpleResponse(socketfd) > 0) {
            write(1, DESCONNECTING_OK, strlen(DESCONNECTING_OK));
            close(socketfd);
            socketfd = -1;
        } else {
            write(1, CONNECTION_NENT, strlen(CONNECTION_NENT));

        }


    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }
}