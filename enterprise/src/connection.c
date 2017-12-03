//
// Created by gabriel on 19/11/17.
//

#include "../include/connection.h"

void sendInfoData() {
    //int socket = *((int*)arg);
    Packet packet, response;
    char *buffer = NULL, convert[10];
    while (1) {
        if ((socketData = createConnectionClient(enterprise.data.port, enterprise.data.ip)) > 0) {
            sprintf(convert, "%d", enterprise.picard.port);
            buffer = createBuffer(3, enterprise.restaurant.name, convert, enterprise.picard.ip);
            packet = createPacket(CONNECT, HEADER_DATPIC, (unsigned short) strlen(buffer), buffer);
            sendSerialized(socketData, packet);
            response = extractIncomingFrame(socketData);

            if (response.type == '1') {
                close(socketData);
                sleep(enterprise.restaurant.seconds);
                //pause();
            } else {
                if (buffer != NULL) {
                    free(buffer);
                }
                exit(EXIT_FAILURE);
                close(socketData);

            }
        }
    }
    //Faltaria tancar el socket.
}

void *connection_dataListener(void *arg) {
   // alarm(enterprise.restaurant.seconds);
    sendInfoData();
    return arg;
}

void connection_executeEnterpriseClient() {
    int error = 0;

    pthread_t thread_id;
    error = pthread_create(&thread_id, NULL, connection_dataListener, NULL);
    if (error != 0) {
        perror("could not create thread");
        close(socketData);
    }
}

void *connection_Picard(void *arg){
    Packet packet;
    int socket = *((int*)arg);
    packet = extractIncomingFrame(socket);
    if(packet.type == '1' && (strcmp(packet.header,HEADER_PICINF) == 0) && packet.length >0){
        sendConnexionOKPacket(socket);
    }else{
        sendConnexionKOPacket(socket);
    }

    return NULL;
}

void connection_createConnectionPicards() {
    parallelHandler(enterprise.picard.port, enterprise.picard.ip,connection_Picard/*,NULL*/);

}
