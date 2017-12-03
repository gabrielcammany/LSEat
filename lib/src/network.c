#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include "../include/network.h"

int createConnectionClient(int portInput, char *ipInput) {

    // comprovem la validesa del port
    if (portInput < 1 || portInput > 65535) {
        fprintf(stderr, ERR_PORT, portInput);
        return -1;
    }
    uint16_t port;
    port = portInput;
    // comprovem la validesa de l'adre￿a ip
    // i la convertim a format binari
    struct in_addr ip_addr;
    if (inet_aton(ipInput, &ip_addr) == 0) {
        fprintf(stderr, ERR_ATON, ipInput, strerror(errno));
        return -1;
    }
    // creem el socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket TCP");
        return -1;
    }
    // especifiquem l'adre￿a del servidor
    struct sockaddr_in s_addr;
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr = ip_addr;

    if (connect(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}

int createConnectionServer(int portInput, char *ipInput) {

    int retVal;

    // we check if the port is alright
    uint16_t port;
    if (portInput < 1 || portInput > 65535) {
        fprintf(stderr, ERR_PORT, portInput);
        return -1;
    }
    port = portInput;

    struct in_addr ip_addr;
    if (inet_aton(ipInput, &ip_addr) != 1) {
        fprintf(stderr, ERR_ATON, ipInput, strerror(errno));
        return -1;
    }

    // now its time to create the socket
    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket TCP");
        return -1;
    }

    struct sockaddr_in s_addr;
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = inet_addr(ipInput);//INADDR_ANY;


    retVal = bind(sockfd, (void *) &s_addr, sizeof(s_addr));
    if (retVal < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    listen(sockfd, 64);
    return sockfd;
}

void serialHandler(int socketfd, void *(*handler)(void *)) {

    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof(c_addr);

    if (socketfd < 0) {
        exit(EXIT_FAILURE);
    }

    while (1) {


        int newsock = accept(socketfd, (void *) &c_addr, &c_len);
        if (newsock < 0) {
            perror("accept");
            close(socketfd);
            exit(EXIT_FAILURE);
        }


        handler((void *) &newsock);
    }
}

void parallelHandler(int port, char *ip, void *(*handler)(void *)/*, void *arg*/) {
    char *buff = NULL;
    int sockfd;

    if (ip != NULL) {
        sockfd = createConnectionServer(port, ip);
    } else {
        sockfd = port;
    }

    pthread_t thread_id;

    while (1) {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);

        int newsock = accept(sockfd, (void *) &c_addr, &c_len);
        if (newsock < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        buff = (char *) malloc(sizeof(char) * strlen(MSG_NEW) + 25);
        sprintf(buff, MSG_NEW, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        write(1, buff, strlen(buff));


        if (pthread_create(&thread_id, NULL, handler, &newsock) < 0) {
            perror("could not create thread");
            close(newsock);
        }
    }
}

int sendSerialized(int socket, Packet packet) {
    char *buffer = NULL;

    unsigned short size = (unsigned short) (SIMPLE_PACKET_LENGTH + strlen(packet.data));

    buffer = (char *) malloc(sizeof(char) * size);
    if (buffer == NULL) {
        return 0;
    }

    memset(buffer, 0, size);

    if (packet.data != NULL) {


        buffer[0] = packet.type;

        memcpy(buffer + sizeof(char), packet.header, HEADER_SIZE);

        //buffer[HEADER_SIZE + TYPE_SIZE] = (char) (packet.length & 0x0F);

        //buffer[HEADER_SIZE + TYPE_SIZE + 1] = (char) ((packet.length & 0xF0) >> 8);
        //memcpy(buffer+HEADER_SIZE + TYPE_SIZE,(char*)packet.length,2);
        buffer[HEADER_SIZE + TYPE_SIZE] = (char) packet.length;
        memcpy(buffer + SIMPLE_PACKET_LENGTH, packet.data, packet.length);

        if (write(socket, buffer, size) == size) {

            free(buffer);
            return 1;
        }

    }


    free(buffer);
    return 0;

}

Packet createPacket(int type, char *header, unsigned short length, char *data) {

    Packet packet;

    //We put \0 in the header so if it has less length we always send 10 bytes
    //We also copy the type, the header and the length this funcions recives
    memset(packet.header, '\0', sizeof(char) * HEADER_SIZE);
    sprintf(&packet.type, "%d", type);
    memcpy(packet.header, header, HEADER_SIZE);
    packet.length = length;

    if (data != NULL) {
        packet.data = (char *) malloc(sizeof(char) * strlen(data));
        if (length > 0) {

            if (packet.data == NULL) {

                packet.type = 0;
                return packet;

            }

            memset(packet.data, 0, sizeof(char) * packet.length);
            memcpy(packet.data, data, packet.length);
        } else {
            packet.data = "\0";
        }

    } else {
        packet.type = 0;
    }


    return packet;

}

int readSimpleResponse(int socket) {

    char header[HEADER_SIZE], type;

    read(socket, &type, sizeof(char));

    switch (type) {
        case CONNECT || DISCONNECT:
            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_CON) == 0);
        case MENU:

            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_ENDMENU) == 0);
        case DISH:

            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_ORD) == 0);
        case DEL_DISH:

            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_ORD) == 0);
        case PAY:

            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_PAY) == 0);
        case UPDATE:

            read(socket, header, HEADER_SIZE * sizeof(char));

            return (strcmp(header, HEADER_NUPDATE) == 0);
        default:
            break;
    }
    return 0;

}

Packet extractIncomingFrame(int socket) {

    char type, header[HEADER_SIZE], *data = NULL;
    unsigned short length = 0;
    Packet packet;

    memset(header, '\0', HEADER_SIZE);

    read(socket, &type, sizeof(char));
    read(socket, &header, sizeof(char) * HEADER_SIZE);


    if (!strcmp(header, HEADER_NCON)) {
        close(socket);

    } else {

        read(socket, &length, sizeof(unsigned short));

        if (length > 0) {
            data = (char *) malloc(sizeof(char) * length);

            if (data != NULL) {

                if (read(socket, data, sizeof(char) * length) > 0) {


                    return createPacket(type - '0', header, length, data);

                }

            }
        }else{
            return createPacket(type - '0', header, length, "\0");

        }
    }
    packet.type = 0;
    return packet;

}

void sendConnexionKOPacket(int socket) {
    Packet aux;
    //if not we send the KO packet to the client
    aux = createPacket(CONNECT, HEADER_NCON, (unsigned short) 0, "\0");
    sendSerialized(socket, aux);
    write(1, ERR_CLIENT, strlen(ERR_CLIENT));
    write(1, '\0', sizeof(char));
    close(socket);

}

void sendConnexionOKPacket(int socket) {
    Packet aux;
    //if not we send the KO packet to the client
    aux = createPacket(CONNECT, HEADER_CON, (unsigned short) 0, "\0");
    sendSerialized(socket, aux);
    /*write(1, ERR_CLIENT, strlen(ERR_CLIENT));
    write(1, '\0', sizeof(char));
    close(socket);*/

}