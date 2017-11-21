//
// Created by gabriel on 31/10/17.
//

#include "../include/connection.h"
#include "../../lib/include/network.h"


char *connection_data(int port, char *ip, char *name) {
	int socket;
	Packet packet;

	packet.data = NULL;

	write(1, CONNECTING, strlen(CONNECTING));
	socket = createConnectionClient(port, ip);

	if (socket < 0) {

		write(1, ERR_CONN, strlen(ERR_CONN));

	} else {

		packet = createPacket(CONNECT, HEADER_PICDAT, (unsigned short) strlen(name), name);

		if (packet.type > 0) {

			if (sendSerialized(socket, packet) > 0) {

				packet = extractIncomingFrame(socket);

				if (packet.type == 1 && !strcmp(packet.header, HEADER_DATPIC)) {

					write(0, CONNECTION_DATA, strlen(CONNECTION_DATA));

				} else {

					write(0, CONNECTION_NDATA, strlen(CONNECTION_NDATA));

				}
			}

		}
	}

	close(socket);

	return packet.data;
}


void extractEnterpriseData(char *data, char **nom, int *port, char **ip) {

	char *token, auxPort[5];

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
	int port, socket = -1;

	extractEnterpriseData(data, &nameEnterprise, &port, &ip);

	if (nameEnterprise != NULL && ip != NULL && port > 0) {
		socket = createConnectionClient(port, ip);

		if (socket < 0) {
			write(1, ERR_CONN, strlen(ERR_CONN));
		} else {

			sprintf(money, "%d", saldo);

			userData = createBuffer(2, nameUser, money);

			if (userData != NULL) {

				Packet packet = createPacket(CONNECT, HEADER_PICINF, (int) strlen(userData), userData);

				if (packet.type > 0) {

					sendSerialized(socket, packet);

					if (readSimpleResponse(socket) > 0) {

						write(0, CONNECTION_ENT, strlen(CONNECTION_ENT));

					} else {

						write(0, CONNECTION_NENT, strlen(CONNECTION_NENT));
						close(socket);
						socket = -1;
					}

				}

			} else {
				write(0, ERR_MEMORY, strlen(ERR_MEMORY));
			}


		}
	} else {
		write(0, ERR_DATA, strlen(ERR_DATA));
	}

	free(userData);
	return socket;


}