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
#include "../../lib/include/menuStructure.h"
#include "../include/controller.h"
#include "../include/picardStructure.h"
#include "../../lib/include/network.h"

#define MIN_FD 2
#define EMPTY_BUCKET (NULL)

int CONNECTION_connectData() {

	Packet packet, response;
	char *buffer = NULL;

	buffer = UTILS_createBuffer(3, enterprise.restaurant.name, enterprise.config.picard_port,
								enterprise.config.picard_ip);

	packet = NETWORK_createPacket(CONNECT, HEADER_DATPIC, (unsigned short) strlen(buffer), buffer);

	NETWORK_sendSerialized(socketData, packet);

	NETWORK_freePacket(&packet);

	response = NETWORK_extractIncomingFrame(socketData);

	if (response.type != (CONNECT)) {

		if (buffer != NULL) {
			free(buffer);
		}

		write(1, ERR_CONNECTION, strlen(ERR_CONNECTION));

		raise(SIGUSR1);

		return -1;

	}


	if (buffer != NULL)free(buffer);

	if(NETWORK_openedSocket(socketData))close(socketData);

	return 1;
}

void *CONNECTION_dataListener(void *arg) {
	char *buffer = NULL;
	Packet packet;
	char convert[10], message[100];
	int limitReconnect = 1;

	while (1) {

		sleep((unsigned int) enterprise.restaurant.seconds);

		if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port),
														 enterprise.config.data_ip)) < 0) {

			sprintf(message,UPDATE_ERR, limitReconnect ,LIMIT_RECONNECT);

			write(1, message, strlen(message));


			if(limitReconnect == LIMIT_RECONNECT){

				write(STDOUT_FILENO,ERR_LIMIT,strlen(ERR_LIMIT));
				break;

			}

			limitReconnect++;

		} else {

			if(limitReconnect > 1){

				write(1,UPDATE_ERR_OK,strlen(UPDATE_ERR_OK)* sizeof(char));
				limitReconnect = 1;

			}
			memset(convert, 0, 10);

			pthread_mutex_lock(&mtx);
			sprintf(convert, "%d", enterprise.clients.elements);
			pthread_mutex_unlock(&mtx);

			buffer = UTILS_createBuffer(2, enterprise.config.picard_port, convert);

			packet = NETWORK_createPacket(UPDATE, HEADER_EUPDATE, (unsigned short) strlen(buffer), buffer);

			if (buffer != NULL)free(buffer);

			NETWORK_sendSerialized(socketData, packet);

			NETWORK_freePacket(&packet);

			packet = NETWORK_extractIncomingFrame(socketData);

			if (packet.type != (UPDATE)) {

				write(1, UPDATE_ERR_DATA, strlen(UPDATE_ERR_DATA));

				if(NETWORK_openedSocket(socketData) > 0){

					close(socketData);

				}

				break;

			}else{

				if((memcmp(packet.header, HEADER_NUPDATE, HEADER_SIZE)==0)){

					write(1, UPDATE_ERR_DATA_REC, strlen(UPDATE_ERR_DATA_REC));

					if(NETWORK_openedSocket(socketData) > 0){

						close(socketData);

					}

					if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port), enterprise.config.data_ip)) <
						0) {

						break;

					}else{

						if (CONNECTION_connectData() < 0) {

							break;

						}else{

							write(1,UPDATE_ERR_REC_OK,strlen(UPDATE_ERR_REC_OK)* sizeof(char));

						}

					}

				}

			}

			NETWORK_freePacket(&packet);

		}

		if(NETWORK_openedSocket(socketData) > 0 && socketData > 0){
			close(socketData);
		}


	}

	pthread_exit(arg);
}

int CONNECTION_executeEnterpriseClient() {
	int error = 0;

	if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port), enterprise.config.data_ip)) <
		0) {

		write(1, ERR_CONNECTION, strlen(ERR_CONNECTION));
		raise(SIGUSR1);

	} else {

		if (CONNECTION_connectData() > 0) {

			if ((error = pthread_create(&enterprise.thread_data, NULL, CONNECTION_dataListener, NULL)) != 0) {

				perror("could not create thread");

			}
			pthread_detach(enterprise.thread_data);
		}
	}
	return error;
}

void *CONNECTION_Picard(void *arg) {
	Packet packet;
	char error[50];
	int socket = *((int *) arg), exit = 0, pos = 0;

	while (!exit) {

		packet = NETWORK_extractIncomingFrame(socket);

		if (packet.type != ERROR_CODE) {

			exit = CONNECTION_analysePacketPicard(socket, packet);

			NETWORK_freePacket(&packet);

		} else {

			pthread_mutex_lock(&mtx);

			if ((pos = PSTRUCTURE_findElement(enterprise.clients, socket)) > 0) {

				sprintf(error, ERR_CONN_PIC, enterprise.clients.bucket[pos].data);
				write(1, error, strlen(error));

				PSTRUCTURE_deleteBucket(&enterprise.clients.bucket[pos]);

			} else {

				write(1, ERR_CONN_CLIENT, strlen(ERR_CONN_CLIENT));

			}

			pthread_mutex_unlock(&mtx);
			break;

		}

	}


	if (socket > MIN_FD)close(socket);

	return NULL;
}

void CONNECTION_createConnectionPicards() {

	NETWORK_parallelHandler(atoi(enterprise.config.picard_port), enterprise.config.picard_ip, CONNECTION_Picard);

}

int CONNECTION_analysePacketPicard(int socket, Packet packet) {
	char *plat = NULL, *units = NULL, *money = NULL, *name = NULL, *buff = NULL;
	int num = 0, vReturn = 0, trobat = 0, i = 0;
	Packet packet1;

	switch (packet.type) {
		case 1:
			if ((memcmp(packet.header, HEADER_PICINF,HEADER_SIZE) == 0) && packet.length > 0) {

				UTILS_extractFromBuffer(packet.data, 2, &name, &money);

				pthread_mutex_lock(&mtx);
				PSTRUCTURE_insert(&enterprise.clients, PSTRUCTURE_createBucket(socket, name, atoi(money), pthread_self()));
				pthread_mutex_unlock(&mtx);

				write(1, "Connectant ", strlen("Connectant ") * sizeof(char));
				write(1, name, strlen(name));
				write(1, "\n", sizeof(char));

				NETWORK_sendOKPacket(socket, CONNECT, HEADER_CON);

			} else {

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

			}

			break;
		case 2:

			if ((memcmp(packet.header, HEADER_PICDAT, HEADER_SIZE)==0) && packet.length > 0) {

				pthread_mutex_lock(&mtx);

                num = PSTRUCTURE_findElement(enterprise.clients,socket);

				if(num > -1){

					if(MSTRUCTURE_isEmpty(enterprise.clients.bucket[num].comanda) > 0){

						MSTRUCTURE_returnCommands(enterprise.clients.bucket[num].comanda,&enterprise.restaurant.menu);

					}

					NETWORK_sendOKPacket(socket, CONNECT, HEADER_CON);


					write(1, "Desconnectant ", strlen("Desconnectant "));
					write(1, packet.data, strlen(packet.data));
					write(1, "\n", sizeof(char));

					vReturn = 1;

					PSTRUCTURE_deletePos(&enterprise.clients, num);

				}else{

					NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

				}


				pthread_mutex_unlock(&mtx);

			} else {

				NETWORK_sendKOPacket(socket, CONNECT, HEADER_NCON);

			}

			break;
		case 3:
			if ((memcmp(packet.header, MENU_PICENT, HEADER_SIZE)==0)) {
				write(1, "Enviant Menu...\n", strlen("Enviant Menu...\n"));
				pthread_mutex_lock(&mtx);

				while(num != enterprise.restaurant.menu.elements ){
					if(enterprise.restaurant.menu.bucket[i].key != EMPTY_BUCKET) {

						if(enterprise.restaurant.menu.bucket[i].number > 0){

							units = (char*)malloc(sizeof(char) * 10);
							sprintf(units,"%d",enterprise.restaurant.menu.bucket[i].number);

							money = (char*) malloc (sizeof(char) * 10);
							sprintf(money,"%d",enterprise.restaurant.menu.bucket[i].data);

							buff = UTILS_createBuffer(
									3,
									enterprise.restaurant.menu.bucket[i].key,
									money,
									units

							);

							packet1 = NETWORK_createPacket(MENU, MENU_DISH, (int) strlen(buff), buff);
							NETWORK_sendSerialized(socket, packet1);

							if(buff!=NULL){free(buff);buff=NULL;}
							if(money!=NULL){free(money);money=NULL;}
							if(units!=NULL){free(units);units=NULL;}

							NETWORK_freePacket(&packet1);
						}

						num++;
					}
					i++;

				}
				pthread_mutex_unlock(&mtx);

				packet1 = NETWORK_createPacket(MENU,END_MENU,0,NULL);

				NETWORK_sendSerialized(socket, packet1);
				NETWORK_freePacket(&packet1);

			} else {

				write(1, "Error en la trama de MENU!\n", strlen("Error en la trama de MENU!\n"));

			}
			break;
		case 4:
			if ((memcmp(packet.header, NEW_ORD,HEADER_SIZE) == 0) && packet.length > 0) {
				//We extract information for each buffer
				UTILS_extractFromBuffer(packet.data, 2, &plat, &units);
				plat = UTILS_toLower(plat);

				//We use mutex to control that only one client has access to the table of picards
				pthread_mutex_lock(&mtx);

				//The key for the table of picards is the socket
				//so we look for the socket of the client that demans one plate
				num = PSTRUCTURE_findElement(enterprise.clients,socket);
				pthread_mutex_unlock(&mtx);

				vReturn = 4;
				//In case we find the picard i our table num will be 0 or more
				if(num >= 0){

					pthread_mutex_lock(&mtx);
					//Trobat again is the position in the table where the element we are looking for is
					if(enterprise.clients.bucket[num].comanda.bucket == NULL){
						enterprise.clients.bucket[num].comanda = MSTRUCTURE_createStructure(MIN_COMMAND);
						trobat = -1;
					}else{
						trobat = MSTRUCTURE_findElement(enterprise.clients.bucket[num].comanda,plat);
					}
					pthread_mutex_unlock(&mtx);

					if(trobat >= 0){
						pthread_mutex_lock(&mtx);

						if ((i = MSTRUCTURE_findElement(enterprise.restaurant.menu,plat)) > -1){

							if((MSTRUCTURE_getPrice(enterprise.restaurant.menu,i) *
								atoi(units)) <= enterprise.clients.bucket[num].number){

								if(		MSTRUCTURE_decrementNum(
										&enterprise.restaurant.menu,
										i/*enterprise.clients.bucket[num].comanda.bucket[trobat].data*/,
										atoi(units)	) > 0){

									MSTRUCTURE_incrementNum(&enterprise.clients.bucket[num].comanda,trobat,atoi(units));

									PSTRUCTURE_calculateMoneyLeft(
											2,
											&enterprise.clients.bucket[num],
											MSTRUCTURE_getPrice(enterprise.restaurant.menu,i),
											atoi(units)
									);

									write(1, "Anotant ", strlen("Anotant "));
									write(1, plat, strlen(plat));
									write(1, " ", sizeof(char));
									write(1, units, strlen(units));
									write(1, "\n", sizeof(char));

									NETWORK_sendOKPacket(socket, DISH, HEADER_ORD);

									vReturn = 0;
								}
							}

						}
						pthread_mutex_unlock(&mtx);

					}else{

						//In case the plate doesnt exist in the picards table of commands
						//tobat will get the value of -1 and then we will proceed to introduce it
						//in this table
						pthread_mutex_lock(&mtx);
						trobat = MSTRUCTURE_findElement(enterprise.restaurant.menu,plat);
						pthread_mutex_unlock(&mtx);

						if(trobat >= 0){

							pthread_mutex_lock(&mtx);
							if((MSTRUCTURE_getPrice(enterprise.restaurant.menu,trobat) *
								atoi(units)) <= enterprise.clients.bucket[num].number) {

								if (MSTRUCTURE_decrementNum(
										&enterprise.restaurant.menu,
										trobat,
										atoi(units)) > 0) {

									MSTRUCTURE_insert(&enterprise.clients.bucket[num].comanda,
													  MSTRUCTURE_createBucket(plat, trobat, atoi(units)));

									PSTRUCTURE_calculateMoneyLeft(
											2,
											&enterprise.clients.bucket[num],
											MSTRUCTURE_getPrice(enterprise.restaurant.menu, trobat),
											atoi(units)
									);

									write(1, "Anotant ", strlen("Anotant "));
									write(1, plat, strlen(plat));
									write(1, " ", sizeof(char));
									write(1, units, strlen(units));
									write(1, "\n", sizeof(char));

									vReturn = 0;

									NETWORK_sendOKPacket(socket, DISH, HEADER_ORD);

								}
							}
							pthread_mutex_unlock(&mtx);


						}

					}

				}

			}

			if(vReturn == 4){
				NETWORK_sendKOPacket(socket, DISH, HEADER_NORD);
				vReturn = 0;
			}
			break;
		case 5:

			if ((memcmp(packet.header, DEL_ORD, HEADER_SIZE) == 0) && packet.length > 0) {

				UTILS_extractFromBuffer(packet.data, 2, &plat, &units);

				pthread_mutex_lock(&mtx);
				//We look for the client who wants to eliminate the plate
				//num is the clients position in out table
				num = PSTRUCTURE_findElement(enterprise.clients,socket);
				pthread_mutex_unlock(&mtx);

				vReturn = 5;

				if(num >= 0){
					//In case the icard exists, we now look for the plate he
					//wants to remove from his demands
					plat = UTILS_toLower(plat);

					pthread_mutex_lock(&mtx);
					//trobat again is the position of the plate in the table
					trobat = MSTRUCTURE_findElement(enterprise.clients.bucket[num].comanda,plat);
					pthread_mutex_unlock(&mtx);

					if(trobat >= 0){

						pthread_mutex_lock(&mtx);

						i = MSTRUCTURE_findElement(enterprise.restaurant.menu,plat);

						if (MSTRUCTURE_decrementNum(&enterprise.clients.bucket[num].comanda,trobat,atoi(units)) > 0){

							MSTRUCTURE_incrementNum(&enterprise.restaurant.menu,i,atoi(units));
							PSTRUCTURE_calculateMoneyLeft(
									1,
									&enterprise.clients.bucket[num],
									MSTRUCTURE_getPrice(enterprise.restaurant.menu,i),
									atoi(units)
							);


							write(1, "Eliminant ", strlen("Eliminant "));
							write(1, plat, strlen(plat));
							write(1, " ", sizeof(char));
							write(1, units, strlen(units));
							write(1, "\n", sizeof(char));


							NETWORK_sendOKPacket(socket, DEL_DISH, HEADER_ORD);

							vReturn = 0;

						}
						pthread_mutex_unlock(&mtx);

					}

				}

			}

			if(vReturn == 5){

				NETWORK_sendKOPacket(socket, DEL_DISH, HEADER_NORD);
				vReturn = 0;

			}

			break;
		case 6:

			if ((strcmp(packet.header, PAY_HEADER) == 0)) {
				pthread_mutex_lock(&mtx);

				num = PSTRUCTURE_findElement(enterprise.clients,socket);

				if ( num >= 0 ){


					write(1, "Notificant factura.\n", strlen("Notificant factura.\n"));

					buff = (char*) malloc (sizeof(char) * 4);
					sprintf(buff,"%d", enterprise.clients.bucket[num].number);

					packet1 = NETWORK_createPacket(PAY,HEADER_PAY,(int)strlen(buff),buff);
					NETWORK_sendSerialized(socket,packet1);
					NETWORK_freePacket(&packet1);

                    if(MSTRUCTURE_isEmpty(enterprise.clients.bucket[num].comanda) > 0){
						MSTRUCTURE_destruct(&enterprise.clients.bucket[num].comanda);
                    }

				}else{
					NETWORK_sendKOPacket(socket,PAY,HEADER_NPAY);
				}

			} else {

				write(1, "Error en la trama de Factura!\n", strlen("Error en la trama de Factura!\n"));

			}
			pthread_mutex_unlock(&mtx);

			if(buff!=NULL){free(buff);buff=NULL;}
			break;

		default:
			break;

	}

	if (money != NULL)free(money);
	if (units != NULL)free(units);
	if (plat != NULL)free(plat);
	if (name != NULL)free(name);

	return vReturn;

}
