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

#define MIN_FD 2


int CONNECTION_connectData() {

	Packet packet, response;
	char *buffer = NULL;

	buffer = UTILS_createBuffer(3, enterprise.restaurant.name, enterprise.config.picard_port, enterprise.config.picard_ip);

	packet = NETWORK_createPacket(CONNECT, HEADER_DATPIC, (unsigned short) strlen(buffer), buffer);

	NETWORK_sendSerialized(socketData, packet);

	NETWORK_freePacket(&packet);

	response = NETWORK_extractIncomingFrame(socketData);

	if (response.type != (CONNECT+'0')) {

		if (buffer != NULL) {
			free(buffer);
		}

		kill(getpid(), SIGUSR1);

	}

	if (buffer != NULL)free(buffer);

	if(socketData > MIN_FD)close(socketData);

	return 1;
}

void *CONNECTION_dataListener(void *arg) {
	char *buffer = NULL;
	Packet packet;
	char convert[10];

	if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port), enterprise.config.data_ip)) < 0) {

		write(1, ERR_CONNECTION, strlen(ERR_CONNECTION));
		raise(SIGUSR1);

	} else {

		if (CONNECTION_connectData() > 0) {

			while (1) {

				sleep((unsigned int) enterprise.restaurant.seconds);

				if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port), enterprise.config.data_ip)) < 0) {

					write(1, UPDATE_ERR, strlen(UPDATE_ERR));

					break;

				} else {

					memset(convert,0,10);

					pthread_mutex_lock(&mtx);
					sprintf(convert,"%d",enterprise.clients.elements);
					pthread_mutex_unlock(&mtx);

					buffer = UTILS_createBuffer(2, enterprise.config.picard_port,convert);

					packet = NETWORK_createPacket(UPDATE, HEADER_EUPDATE, (unsigned short) strlen(buffer), buffer);

					if(buffer != NULL)free(buffer);

					NETWORK_sendSerialized(socketData, packet);

					NETWORK_freePacket(&packet);

					packet = NETWORK_extractIncomingFrame(socketData);

					if (packet.type != (UPDATE+'0')) {

						write(1, UPDATE_ERR, strlen(UPDATE_ERR));

						break;

					}

					NETWORK_freePacket(&packet);

					if(socketData > MIN_FD)close(socketData);

				}



			}

		}

	}

	return arg;
}

int CONNECTION_executeEnterpriseClient() {
	int error = 0;

	error = pthread_create(&enterprise.thread_data, NULL, CONNECTION_dataListener, NULL);

	if (error != 0) {

		perror("could not create thread");

	}
	return error;
}

void *CONNECTION_Picard(void *arg) {
	Packet packet;
	char error[50];
	int socket = *((int *) arg), exit = 0, pos = 0;

	while (!exit) {

		packet = NETWORK_extractIncomingFrame(socket);

		if (packet.type > 0) {

			exit = CONNECTION_analysePacketPicard(socket, packet);

		} else {

			if((pos = PSTRUCTURE_findElement(enterprise.clients,socket)) > 0){

				pthread_mutex_lock(&mtx);

				sprintf(error, ERR_CONN_PIC, enterprise.clients.bucket[pos].data);
				write(1, error, strlen(error));
				PSTRUCTURE_deleteBucket(&enterprise.clients.bucket[pos]);

				pthread_mutex_unlock(&mtx);


			}else{

				write(1, ERR_CONN_CLIENT, strlen(ERR_CONN_CLIENT));

			}

			break;

		}

	}

	if(socket > MIN_FD)close(socket);

	return NULL;
}

void CONNECTION_createConnectionPicards() {

	NETWORK_parallelHandler(atoi(enterprise.config.picard_port), enterprise.config.picard_ip, CONNECTION_Picard);

}

int CONNECTION_analysePacketPicard(int socket, Packet packet) {
	char *plat = NULL, *units = NULL, *money = NULL , *name = NULL;
	int num= 0 ;

	switch (packet.type) {
		case '1':
			if ((strcmp(packet.header, HEADER_PICINF) == 0) && packet.length > 0) {

				UTILS_extractFromBuffer(packet.data, 2, &name, &money);
				num = atoi(money);

				pthread_mutex_lock(&mtx);
				PSTRUCTURE_insert(&enterprise.clients,PSTRUCTURE_createBucket(socket,name,num, pthread_self()));
				pthread_mutex_unlock(&mtx);

				write(1, name, strlen(name));
				write(1, "\n", sizeof(char));

				NETWORK_sendOKPacket(socket, CONNECT, HEADER_CON);

			} else {

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

			}

			break;
		case '2':

			if ((strcmp(packet.header, HEADER_PICDAT) == 0) && packet.length > 0) {

				if (PSTRUCTURE_delete(&enterprise.clients,socket) > 0) {

					NETWORK_sendOKPacket(socket, UPDATE, HEADER_UPDATE);

					write(1, "Desconnectant ", strlen("Desconnectant "));
					write(1, packet.data, strlen(packet.data));
					write(1, "\n", sizeof(char));

					return 1;

				} else {

					NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				}

			} else {

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

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

				UTILS_extractFromBuffer(packet.data, 2, &units, &plat);

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

				UTILS_extractFromBuffer(packet.data, 2, &units, &plat);

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

		default:
			break;

	}

	if (money != NULL)free(money);
	if (units != NULL)free(units);
	if (plat != NULL)free(plat);
	if (name != NULL)free(name);

	NETWORK_freePacket(&packet);

	return 0;

}
