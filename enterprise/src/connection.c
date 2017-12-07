/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/connection.h"

void sendInfoData() {
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
                break;
            }
        } else {
            break;
        }
    }

    close(socketData);
}

//se puede juntar en 1
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

void *connection_Picard(void *arg) {
    Packet packet;
    int socket = *((int *) arg), exit = 0;
    while (!exit) {
        packet = extractIncomingFrame(socket);
        exit = analysePacketPicard(socket, packet);
    }


    return NULL;
}

void connection_createConnectionPicards() {
    parallelHandler(enterprise.picard.port, enterprise.picard.ip, connection_Picard);

}

int analysePacketPicard(int socket, Packet packet) {
    Element picard;
    char *plat = NULL, *units = NULL, *money = NULL;
    switch (packet.type) {
        case '1':
            if ((strcmp(packet.header, HEADER_PICINF) == 0) && packet.length > 0) {

                picard.socket = socket;
                extractFromBuffer(packet.data, 2, &picard.name, &money);
                picard.money = atoi(money);
                picard.factura = 0;

                LLISTA_insereix(&picards, picard);

                write(1, "Connectat ", strlen("Connectat "));
                write(1, picard.name, strlen(picard.name));
                write(1, "\n", sizeof(char));

                sendConnexionOKPacket(socket, CONNECT);

            } else {
                sendConnexionKOPacket(socket, CONNECT);
            }
            break;
        case '2':

            if ((strcmp(packet.header, HEADER_PICDAT) == 0) && packet.length > 0) {

                if (controller_eliminaPicard(packet.data) > 0) {

                    sendConnexionOKPacket(socket, CONNECT);

                    write(1, "Desconnectant ", strlen("Desconnectant "));
                    write(1, packet.data, strlen(packet.data));
                    write(1, "\n", sizeof(char));
                    close(socket);

                    return 1;

                } else {
                    sendConnexionKOPacket(socket, CONNECT);

                }
            } else {
                sendConnexionKOPacket(socket, CONNECT);

            }
            break;
        case '3':
            if ((strcmp(packet.header, MENU_PICENT) == 0)) {
                write(1, "Enviant Menu...\n", strlen("Enviant Menu...\n"));

            } else {
                write(1, "Error en la trama de MENU!\n", strlen("Error en la trama de MENU!\n"));

            }
            break;
        case '4':
            if ((strcmp(packet.header, NEW_ORD) == 0) && packet.length > 0) {

                extractFromBuffer(packet.data, 2, &units, &plat);

                write(1, "Anotant ", strlen("Anotant "));
                write(1, plat, strlen(plat));
                write(1, " ", sizeof(char));
                write(1, units, strlen(units));
                write(1, "\n", sizeof(char));

            } else {
                write(1, "Error en la trama de DEMANA!\n", strlen("Error en la trama de DEMANA!\n"));

            }
            break;
        case '5':
            if ((strcmp(packet.header, DEL_ORD) == 0) && packet.length > 0) {

                extractFromBuffer(packet.data, 2, &units, &plat);

                write(1, "Eliminant ", strlen("Eliminant "));
                write(1, plat, strlen(plat));
                write(1, " ", sizeof(char));
                write(1, units, strlen(units));
                write(1, "\n", sizeof(char));

            } else {
                write(1, "Error en la trama de ELIMINA!\n", strlen("Error en la trama de ELIMINA!\n"));
            }
            break;
        case '6':

            if ((strcmp(packet.header, PAY_HEADER) == 0)) {

                write(1, "Notificant factura: ", strlen("Notificant factura: "));

            } else {
                write(1, "Error en la trama de Factura!\n", strlen("Error en la trama de Factura!\n"));
            }
            break;

    }

    if (money != NULL)free(money);
    if (units != NULL)free(units);
    if (plat != NULL)free(plat);

    return 0;

}
