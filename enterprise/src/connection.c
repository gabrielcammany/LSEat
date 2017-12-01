//
// Created by gabriel on 19/11/17.
//

#include "../include/connection.h"

void *connection_handlerData(){
    //int socket = *((int*)arg);
    Packet packet;
    char *buffer = NULL;

    while(1){
        buffer = createBuffer(3,enterprise.restaurant.name,enterprise.picard.port,enterprise.picard.ip);
        packet = createPacket(CONNECT,ENT_INF,(unsigned short) strlen(buffer), buffer);
        sendSerialized(socketData,packet);
        alarm(enterprise.restaurant.seconds);
        pause();
    }

    //Faltaria tancar el socket.
}

void *connection_dataListener(void *socket) {
    serialHandler(socketPic, connection_handlerData);
    return socket;
}

void connection_executeEnterprise(){
    if((socketData = createConnectionClient(enterprise.data.port,enterprise.data.ip)) > 0){
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_dataListener, NULL) < 0) {
            perror("could not create thread");
            close(socketPic);
        }
    }
}