//
// Created by gabriel on 19/11/17.
//

#include <signal.h>
#include "../include/connection.h"

void addEnterprise() {

}


void *connection_handlerEnterprise(void *arg) {

}

void *connection_handlerClient(void *arg) {

	int socket = *(int *) arg;
	Packet packet, aux;
	char convert[10], *buffer;

	packet = extractIncomingFrame(socket);

	if (packet.type != 1 || strcmp(packet.header, HEADER_PICINF) == 0) {

		close(socket);
		return NULL;

	}

	write(0, CONNECTING, strlen(CONNECTING));
	write(0, '\0', sizeof(char));

	write(0, packet.data, packet.length * sizeof(char));
	write(0, '\0', sizeof(char));

	sprintf(convert, "%d", enterprise.port);

	buffer = createBuffer(3, enterprise.name, convert, enterprise.IP);

	if (buffer == NULL) {

		close(socket);
		return NULL;

	}

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
}

void dNetwork_executeData(int portE, int portP, char* ip){
	pthread_t thread_id;

	if((socketPic = createConnectionServer(portE, ip) > 0){
		if (pthread_create(&thread_id, NULL, connection_clientListener, (void *)socketPic) < 0) {
			perror("could not create thread");
			exit(EXIT_FAILURE);
		}
	}

	if((socketEnt = createConnectionServer(portE, ip) > 0))
		serialHandler(socketEnt, connect);

	kill(getpid(),SIGUSR1);
}