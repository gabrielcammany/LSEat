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
#include "../include/dataStructure.h"


void *CONNECTION_handlerEnterprise(void *arg) {

	Packet packet;
	int socket = *((int *) arg);

	char *number = NULL, *port = NULL, *aux = NULL;

	packet = NETWORK_extractIncomingFrame(socket);

	if (packet.length > 0) {

		switch (packet.type) {
			case '1':

				if (strcmp(packet.header, HEADER_DATPIC) == 0) {

					aux = (char *) calloc(packet.length,sizeof(char) * packet.length);

					if (aux != NULL) {

						memcpy(aux, packet.data, sizeof(char) * packet.length);

						UTILS_extractFromBuffer(aux, 2, &number, &port);

						HASH_insert(&enterprise, HASH_createBucket(atoi(port), packet.data, 0));

						NETWORK_sendOKPacket(socket, CONNECT, HEADER_CON);

						free(aux);
						free(port);
						free(number);

					} else {

						NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

					}



				} else {

					NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				}


				break;
			case '7':

				if (strcmp(packet.header, HEADER_EUPDATE) == 0) {

					UTILS_extractFromBuffer(packet.data, 2, &port, &number);

					HASH_insert(&enterprise, HASH_createBucket(atoi(port), NULL, atoi(number)));

					NETWORK_sendOKPacket(socket, UPDATE, HEADER_UPDATE);

					free(port);
					free(number);

				} else {

					NETWORK_sendKOPacket(socket, UPDATE, HEADER_NUPDATE);

				}


				break;
			default:

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				break;

		}

		NETWORK_freePacket(&packet);

	} else {

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

	}
	close(socket);

	return arg;
}

void *CONNECTION_handlerClient(void *arg) {

	int socket = *((int *) arg), size;
	Packet packet, aux;

	//We extract the information of the clients packet
	packet = NETWORK_extractIncomingFrame(socket);

	//We know there will be only the type 1 of packet from clients
	if (packet.type != '1' || strcmp(packet.header, HEADER_PICINF) == 0) {

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);
		NETWORK_freePacket(&packet);

		return NULL;

	}

	//Because is type 1 we know that data contains the client name
	write(1, CONNECTING, strlen(CONNECTING));
	write(1, packet.data, sizeof(char) * packet.length);
	write(1, "\n\0", sizeof(char) * 2);

	//we check that struct enterprise isnt empty

	if (!enterprise.elements) {

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);
		write(1, DCONNECT_ERR, strlen(DCONNECT_ERR));
		write(1, packet.data, sizeof(char) * packet.length);
		write(1, "\n\0", sizeof(char) * 2);

	} else {

		size = (int) strlen(enterprise.bucket[enterprise.number].data);

		//we create packet to response client
		aux = NETWORK_createPacket(CONNECT, HEADER_DATPIC,size,
								   enterprise.bucket[enterprise.number].data);

		if (aux.type > 0) {

			//if everything is ok then we send information of the enterprise back to the client
			NETWORK_sendSerialized(socket, aux);
			write(1, DISCONNECTING, strlen(DISCONNECTING));
			write(1, packet.data, sizeof(char) * packet.length);
			write(1, "\n\0", sizeof(char) * 2);

		} else {

			NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

			write(1, DCONNECT_ERR, strlen(DCONNECT_ERR));
			write(1, packet.data, sizeof(char) * packet.length);
			write(1, "\n\0", sizeof(char) * 2);

		}

		NETWORK_freePacket(&aux);

	}

	NETWORK_freePacket(&packet);

	close(socket);
	return NULL;
}

void *CONNECTION_clientListener(void *socket) {
	write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));
	NETWORK_serialHandler(socketPic, CONNECTION_handlerClient);
	return socket;
}

void CONNECTION_executeData(int portE, int portP, char *ip) {

	enterprise = HASH_createTable(MAX_ENTERPRISES);

	if ((socketPic = NETWORK_createConnectionServer(portP, ip)) > 0) {

		if (pthread_create(&thread_id, NULL, CONNECTION_clientListener, NULL) < 0) {
			perror("could not create thread");

			raise(SIGUSR1);

		}

	} else {
		raise(SIGUSR1);
	}

	if ((socketEnt = NETWORK_createConnectionServer(portE, ip)) > 0) {

		NETWORK_serialHandler(socketEnt, CONNECTION_handlerEnterprise);

	}

	raise(SIGUSR1);
}