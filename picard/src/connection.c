//
// Created by gabriel on 31/10/17.
//

#include "../include/connection.h"


char *connection_data(int port, char *ip, char *name) {
	//int socketfd;
	Packet packet;

	packet.data = NULL;

	write(1, CONNECTING, strlen(CONNECTING));

	//Returns the socketfd of the client with one of the servers
	socketfd = createConnectionClient(port, ip);

	if (socketfd < 0) {

		write(1, ERR_CONN, strlen(ERR_CONN));

	} else {
		//We serialize the packet with the information
		packet = createPacket(CONNECT, HEADER_PICDAT, (unsigned short) strlen(name), name);

		if (packet.type > 0) {

			if (sendSerialized(socketfd, packet) > 0) {
				packet = extractIncomingFrame(socketfd);
                analyseDataPacket(packet);

			}

		}
	}

	close(socketfd);

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
	int port; //socketfd = -1;

	extractEnterpriseData(data, &nameEnterprise, &port, &ip);

	if (nameEnterprise != NULL && ip != NULL && port > 0) {
		socketfd = createConnectionClient(port, ip);

		if (socketfd < 0) {
			write(1, ERR_CONN, strlen(ERR_CONN));
		} else {

			sprintf(money, "%d", saldo);

			userData = createBuffer(2, nameUser, money);

			if (userData != NULL) {

				Packet packet = createPacket(CONNECT, HEADER_PICINF, (int) strlen(userData), userData);

				if (packet.type > 0) {

					sendSerialized(socketfd, packet);

					if (readSimpleResponse(socketfd) > 0) {

						write(1, CONNECTION_ENT, strlen(CONNECTION_ENT));

					} else {

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

int analyseDataPacket(Packet packet){

    switch (packet.type){
        case '1':
            if (packet.length == 0){
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
		i++;j++;
		port = (char *) realloc(port, sizeof(char) * (j + 1));
	}

	enterprise.port = atoi(port);
	i++;
	j=0;
	enterprise.ip = (char *) malloc(sizeof(char));
	while (packet.data[i] != '&' && i < packet.length) {
		enterprise.ip[j] = packet.data[i];
		i++;j++;
		enterprise.ip = (char *) realloc(enterprise.ip, sizeof(char) * (j + 1));
	}

	return enterprise;
}