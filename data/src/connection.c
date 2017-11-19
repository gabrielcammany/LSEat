//
// Created by gabriel on 19/11/17.
//

#include "../include/connection.h"

void* connection_Handler(void* arg){

	int socket = *(int*)arg;
	Packet packet, aux;
	char convert[10], *buffer, msg[30];

	packet = extractIncomingFrame(socket);
	if(packet.type != 1 || strcmp(packet.header,HEADER_CON) != 0){

		close (socket);
		return NULL;

	}

	write(0, CONNECTING, strlen(CONNECTING));
	write(0,packet.data, strlen(packet.data) * sizeof(char));
	write(0,'\0', sizeof(char));

	sprintf(convert,"%d", enterprise.port);

	buffer = createBuffer(3, enterprise.name, convert, enterprise.IP);

	if(buffer == NULL){

		close (socket);
		return NULL;

	}

	aux = createPacket(CONNECT,HEADER_CON,(int)strlen(buffer),buffer);

	if(aux.type > 0){

		sendSerialized(socket, aux );
		write(0, DISCONNECTING, strlen(DISCONNECTING));
		write(0, packet.data, strlen(packet.data) *  sizeof(char));
		write(0,'\0', sizeof(char));

	}else{

		write(0, ERR_CLIENT, strlen(ERR_CLIENT));
		write(0, packet.data, strlen(packet.data) *  sizeof(char));
		write(0,'\0', sizeof(char));

	}

	free(buffer);
	return NULL;
}