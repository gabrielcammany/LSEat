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
#include "../../lib/include/network.h"

char *connection_data(int port, char *ip, char *name) {
	Packet packet;
	packet.data = NULL;

	//Message to make the user know we are connecting to Data
	write(1, CONNECTING, strlen(CONNECTING));

	//Returns the file descriptor to talk with Data server
	socketfd = NETWORK_createConnectionClient(port, ip);


	if (socketfd < 0) {
		write(1, ERR_CONN, strlen(ERR_CONN));

	} else {
		//We create a packet and fill it with information
		packet = NETWORK_createPacket(CONNECT, HEADER_PICDAT, (unsigned short) strlen(name), name);

		if (packet.type > 0) {

			//then we send the packet serialized
			if (NETWORK_sendSerialized(socketfd, packet) > 0) {

				NETWORK_freePacket(&packet);

				//we receive the response
				packet = NETWORK_extractIncomingFrame(socketfd);
				//we analyse the information we have recieved

				if(packet.type != ERROR_CODE){

					if(CONNECTION_analyseDataPacket(packet) < 0){

						close(socketfd);
						socketfd = -1;
						NETWORK_freePacket(&packet);

						return NULL;

					}

				}

			}


		}

	}

	close(socketfd);

	return packet.data;
}


void CONNECTION_extractEnterpriseData(char *data, char **nom, int *port, char **ip) {
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
	CONNECTION_extractEnterpriseData(data, &nameEnterprise, &port, &ip);


	if (nameEnterprise != NULL && ip != NULL && port > 0) {
		//we create connection between Picard and Enterprise
		socketfd = NETWORK_createConnectionClient(port, ip);

		if (socketfd < 0) {
			write(1, ERR_CONN, strlen(ERR_CONN));
		} else {
			//we create a buffer with all the information
			//of the picard in order to create a packet with it
			//and after serializating it and send it we wait for a response
			sprintf(money, "%d", saldo);
			userData = UTILS_createBuffer(2, nameUser, money);

			if (userData != NULL) {

				Packet packet = NETWORK_createPacket(CONNECT, HEADER_PICINF, (int) strlen(userData), userData);

				if (packet.type > 0) {

					NETWORK_sendSerialized(socketfd, packet);

					if (NETWORK_readSimpleResponse(socketfd) > 0) {

						write(1, CONNECTION_ENT, strlen(CONNECTION_ENT));

					} else {
						//If something goes wrong we will disconnect
						write(1, CONNECTION_NENT, strlen(CONNECTION_NENT));
						close(socketfd);
						socketfd = -1;
					}

				}

				NETWORK_freePacket(&packet);
				free(userData);

			} else {
				write(0, ERR_MEMORY, strlen(ERR_MEMORY));
			}


		}
	} else {
		write(1, ERR_DATA, strlen(ERR_DATA));
	}

	if(nameEnterprise != NULL)free(nameEnterprise);
	if(ip != NULL)free(ip);


	return socketfd;
}

int CONNECTION_analyseDataPacket(Packet packet) {

	if (packet.type == '1' && !packet.length) {

		//If there is any error during the connection then we will recive a length of 0
		write(1, CONNECTION_NDATA, strlen(CONNECTION_NDATA));

		return 0;

	} else {

		write(1, CONNECTION_DATA, strlen(CONNECTION_DATA));

	}

	return 1;

}

void CONNECTION_requestMenuEnterprise() {

	if (socketfd > 1) {

		Packet packet = NETWORK_createPacket(MENU, MENU_PICENT, 0, NULL);

		NETWORK_sendSerialized(socketfd, packet);

		NETWORK_freePacket(&packet);

	} else {

		write(1, ERR_CONN, strlen(ERR_CONN));

	}

}

void CONNECTION_deleteDishMenu(char **data) {

	if (socketfd > 1) {

		char *buffer = UTILS_createBuffer(2, data[1], data[0]);

		if(buffer != NULL){

			Packet packet = NETWORK_createPacket(DEL_DISH, DEL_ORD, (unsigned short) strlen(buffer), buffer);
			NETWORK_sendSerialized(socketfd, packet);
			NETWORK_freePacket(&packet);

			if(NETWORK_sendSerialized(socketfd, packet) < 0){

				close(socketfd);
				write(1, ERR_CONN, strlen(ERR_CONN));
				socketfd = -1;

			}

			if (NETWORK_readSimpleResponse(socketfd) > 0) {
				write(1, NO_DPLAT, strlen(NO_DPLAT));
			}

			free(buffer);

		}else{

			write(1, ERR_CONN, strlen(ERR_CONN));

		}

	} else {

		write(1, ERR_CONN, strlen(ERR_CONN));

	}
}

void CONNECTION_payEnterprise() {

	if (socketfd > 1) {

		Packet packet = NETWORK_createPacket(PAY, PAY_HEADER, (unsigned short) 0, NULL);

		NETWORK_sendSerialized(socketfd, packet);

		NETWORK_freePacket(&packet);

	} else {

		write(1, ERR_CONN, strlen(ERR_CONN));

	}
}

void CONNECTION_takeNoteEnterprise(char **data) {

	if (socketfd > 2) {

		char *buffer = UTILS_createBuffer(2, data[1], data[0]);

		Packet packet = NETWORK_createPacket(DISH, NEW_ORD, (unsigned short) strlen(buffer), buffer);

		if(NETWORK_sendSerialized(socketfd, packet) < 0){

			close(socketfd);
			write(1, ERR_CONN, strlen(ERR_CONN));
			socketfd = -1;

		}

		if (NETWORK_readSimpleResponse(socketfd) > 0) {
			write(1, NO_PLAT, strlen(NO_PLAT));
		}


		NETWORK_freePacket(&packet);


	} else {

		write(1, ERR_CONN, strlen(ERR_CONN));

	}

	if(data[0] != NULL)free(data[0]);
	if(data[1] != NULL)free(data[1]);
	free(data);


}

void CONNECTION_disconnectEnterprise(char *nom) {
	if (socketfd > 2) {
		Packet packet = NETWORK_createPacket(DISCONNECT, HEADER_PICDAT, (unsigned short) strlen(nom), nom);

		NETWORK_sendSerialized(socketfd, packet);

		if (NETWORK_readSimpleResponse(socketfd) > 0) {

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