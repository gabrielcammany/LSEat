//
// Created by gabriel on 19/11/17.
//

#include <signal.h>
#include "../include/connection.h"

void addEnterprise() {

	enterprise = (Enterprise*)malloc(sizeof(Enterprise));
	if(enterprise == NULL){
		exit(EXIT_FAILURE);
	}

	enterprise->port = 8063;
	enterprise->ip = "127.0.0.1";
	enterprise->numberClients = 0;
	enterprise->name = "Prova";

}


void *connection_handlerEnterprise(void *arg) {
	return NULL;
}

void *connection_handlerClient(void *arg) {

	int socket = socketPic;
	Packet packet, aux;
	char convert[10], *buffer;

	packet = extractIncomingFrame(socket);

	if (packet.type != 1 || strcmp(packet.header, HEADER_PICINF) == 0) {

		close(socket);
		return NULL;

	}

	write(0, CONNECTING, strlen(CONNECTING));
	write(0, '\0', sizeof(char));

	sprintf(convert, "%d", enterprise->port);

	buffer = createBuffer(3, enterprise->name, convert, enterprise->ip);

	if (buffer == NULL) {

		printf("Error\n");

		close(socket);
		return NULL;

	}

	printf("Buffere: -%s-\n",buffer);

	aux = createPacket(CONNECT, HEADER_CON, (unsigned short) strlen(buffer), buffer);

	if (aux.type > 0) {

		sendSerialized(socket, aux);
		write(0, DISCONNECTING, strlen(DISCONNECTING));
		write(0, packet.data, strlen(packet.data) * sizeof(char));
		write(0, '\0', sizeof(char));

	} else {

		write(0, ERR_CLIENT, strlen(ERR_CLIENT));
		write(0, packet.data, strlen(packet.data) * sizeof(char));
		write(0, '\0', sizeof(char));

	}

	free(buffer);
	return NULL;
}

void* connection_clientListener(void *socket){

	serialHandler(*((int*)socket),connection_handlerClient);
	return NULL;
}

void dNetwork_executeData(int portE, int portP, char* ip){
	int socketPic;

	if((socketPic = createConnectionServer(portP, ip)) > 0){

		serialHandler(socketPic,connection_handlerClient);

	}

	/*if((socketEnt = createConnectionServer(portE, ip) > 0)) {
		serialHandler(socketEnt, connection_handlerClient);
	}

	kill(getpid(),SIGUSR1);*/
}