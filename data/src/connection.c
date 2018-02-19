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
	int socket = *((int *) arg), pos = 0;

	char *number = NULL, *port = NULL, *aux = NULL, text[20];

	packet = NETWORK_extractIncomingFrame(socket);

	if (packet.length > 0) {

		switch (packet.type) {
			case 1:

				if (strcmp(packet.header, HEADER_DATPIC) == 0) {

					aux = (char *) calloc(packet.length,sizeof(char) * packet.length);

					if (aux != NULL) {

						memcpy(aux, packet.data, sizeof(char) * packet.length);

						UTILS_extractFromBuffer(aux, 2, &number, &port);
						write(1, "Connectant ", strlen("Connectant ") * sizeof(char));
						write(1, number, strlen(number) * sizeof(char));
						write(1, "\n", sizeof(char));

						pthread_mutex_lock(&mtx);
						HASH_insert(&enterprise, HASH_createBucket(atoi(port), packet.data, 0));
						pthread_mutex_unlock(&mtx);
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
			case 7:

				if (strcmp(packet.header, HEADER_EUPDATE) == 0) {

					UTILS_extractFromBuffer(packet.data, 2, &port, &number);

					pthread_mutex_lock(&mtx);
					HASH_insert(&enterprise, HASH_createBucket(atoi(port), NULL, atoi(number)));
					pthread_mutex_unlock(&mtx);

					NETWORK_sendOKPacket(socket, UPDATE, HEADER_UPDATE);

					free(port);
					free(number);

				} else {

					NETWORK_sendKOPacket(socket, UPDATE, HEADER_NUPDATE);

				}


				break;
			case 2:

				if (strcmp(packet.header, HEADER_DATPIC) == 0) {


					if((pos = HASH_findElement(enterprise, atoi(packet.data))) > 0){

						memset(text,0,20);

						pthread_mutex_lock(&mtx);
						UTILS_extractFromBuffer(enterprise.bucket[pos].data, 1, &port);
						pthread_mutex_unlock(&mtx);

						sprintf(text,DISC_ENT,port);

						write(1,text, strlen(text));

						pthread_mutex_lock(&mtx);
						if(HASH_deletePos(&enterprise,pos) > 0){
							NETWORK_sendOKPacket(socket, DISCONNECT, HEADER_DATPIC);
						}else{
							NETWORK_sendKOPacket(socket, DISCONNECT, HEADER_DATPIC);
						}
						pthread_mutex_unlock(&mtx);


						free(port);
						free(number);

					}else{

						NETWORK_sendKOPacket(socket, DISCONNECT, HEADER_DATPIC);

					}

				} else {

					NETWORK_sendKOPacket(socket, DISCONNECT, HEADER_DATPIC);

				}

				break;

			default:

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				break;

		}


	} else {

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

	}

	NETWORK_freePacket(&packet);

	if(socket > 2)close(socket);

	return NULL;
}

void *CONNECTION_handlerClient(void *arg) {

	int socket = *((int *) arg), size = 0, number = 0;
	Packet packet, aux;
	char *name = NULL, *port = NULL;
	//We extract the information of the clients packet
	packet = NETWORK_extractIncomingFrame(socket);
	//We know there will be only the type 1 of packet from clients
	if ((packet.type != 1 && packet.type != 8) || strcmp(packet.header, HEADER_PICINF) == 0) {

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);
		NETWORK_freePacket(&packet);

		return NULL;

	}

	if(packet.type == 8){
		UTILS_extractFromBuffer(packet.data,2,&name,&port);
		number = atoi(port);
		if(number > 0){
			pthread_mutex_lock(&mtx);
			HASH_delete(&enterprise,number);
			pthread_mutex_unlock(&mtx);
		}

		if(name != NULL){
			free(name);
			name = NULL;
		}

	}

	//Because is type 1 we know that data contains the client name
	write(1, CONNECTING, strlen(CONNECTING));
	write(1, packet.data, sizeof(char) * strlen(packet.data));
	write(1, "\n\0", sizeof(char) * 2);

	//we check that struct enterprise isnt empty

	pthread_mutex_lock(&mtx);
	if (enterprise.elements == 0) {
		pthread_mutex_unlock(&mtx);

		NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);
		write(1, DCONNECT_ERR, strlen(DCONNECT_ERR));
		write(1, packet.data, sizeof(char) * strlen(packet.data));
		write(1, "\n\0", sizeof(char) * 2);

	} else {

		if(enterprise.bucket[enterprise.number].data != NULL){

			size = (int) strlen(enterprise.bucket[enterprise.number].data);

			//we create packet to response client
			aux = NETWORK_createPacket(CONNECT, HEADER_DATPIC,size,
									   enterprise.bucket[enterprise.number].data);
			pthread_mutex_unlock(&mtx);

			if (aux.type > 0) {

				//if everything is ok then we send information of the enterprise back to the client
				NETWORK_sendSerialized(socket, aux);
				write(1, DISCONNECTING, strlen(DISCONNECTING));
				write(1, packet.data, sizeof(char) * strlen(packet.data));
				write(1, "\n\0", sizeof(char) * 2);

			} else {

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				write(1, DCONNECT_ERR, strlen(DCONNECT_ERR));
				write(1, packet.data, sizeof(char) * strlen(packet.data));
				write(1, "\n\0", sizeof(char) * 2);

			}

			NETWORK_freePacket(&aux);

		}else{
			HASH_updateNumber(&enterprise);
			pthread_mutex_unlock(&mtx);

			NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);
			write(1, DCONNECT_ERR, strlen(DCONNECT_ERR));
			write(1, packet.data, sizeof(char) * strlen(packet.data));
			write(1, "a\n\0", sizeof(char) * 3);

		}
	}

	NETWORK_freePacket(&packet);

	if(port != NULL)free(port);
	close(socket);

	return NULL;
}

void *CONNECTION_clientListener(void *socket) {
	write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));

	NETWORK_serialHandler(socketPic, CONNECTION_handlerClient);

	return socket;
}

void CONNECTION_executeData(int portE, int portP, char *ip) {


	if ((socketEnt = NETWORK_createConnectionServer(portE, ip)) > 0) {
		if ((socketPic = NETWORK_createConnectionServer(portP, ip)) > 0) {

			if (pthread_create(&thread_id, NULL, CONNECTION_clientListener, &socketPic) < 0) {
				perror("could not create thread");

			}

			NETWORK_serialHandler(socketEnt, CONNECTION_handlerEnterprise);

		}


	}
	raise(SIGUSR1);
}