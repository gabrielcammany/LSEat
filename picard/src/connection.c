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
#include "../include/controller.h"
#include "../../lib/include/menuStructure.h"

char *CONNECTION_data(int option, int port, char *ip, char *name) {
    Packet packet;
    packet.data = NULL;
    char *buff = NULL, cport[6];
    //Message to make the user know we are connecting to Data
    write(1, CONNECTING, strlen(CONNECTING));

    //Returns the file descriptor to talk with Data server
    socketfd = NETWORK_createConnectionClient(port, ip);

    if (socketfd < 2) {
        write(1, ERR_CONN, strlen(ERR_CONN) * sizeof(char));

    } else {
        //We create a packet and fill it with information
        if(option == 1){
            packet = NETWORK_createPacket(CONNECT, HEADER_PICDAT, (unsigned short) strlen(name), name);

        }else{
            sprintf(cport,"%d",lseat.enterprise.port);

            buff = UTILS_createBuffer(2,lseat.client.nom,cport);
            packet = NETWORK_createPacket(RECONNECT, HEADER_PICDAT, (unsigned short) strlen(buff), buff);

			free(buff);
        }

        if (packet.type > 0) {

            //then we send the packet serialized
            if (NETWORK_sendSerialized(socketfd, packet) > 0) {

				NETWORK_freePacket(&packet);

                //we receive the response
                packet = NETWORK_extractIncomingFrame(socketfd);
                //we analyse the information we have recieved

                if (packet.type != ERROR_CODE) {

                    if (CONNECTION_analyseDataPacket(packet) < 0) {

                        close(socketfd);
                        socketfd = -1;
                        NETWORK_freePacket(&packet);

                        return NULL;

                    }
                } else {
                    return NULL;
                }

            }else{
				NETWORK_freePacket(&packet);

				return NULL;
            }


        } else {
            return NULL;
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


int CONNECTION_enterprise(char *data, char *nameUser, int saldo) {
    char *ip = NULL, *userData = NULL, money[10], *port = NULL;

    //First we extract the information
    //from enterprise that Data sent us
    if(lseat.enterprise.name!= NULL){free(lseat.enterprise.name);lseat.enterprise.name=NULL;}
    if(lseat.enterprise.port!= 0)lseat.enterprise.port=0;

    UTILS_extractFromBuffer(data, 3,&lseat.enterprise.name, &port, &ip);

	free(data);

    lseat.enterprise.port = atoi(port);

    if (lseat.enterprise.name != NULL && ip != NULL && lseat.enterprise.port > 0) {
        //we create connection between Picard and Enterprise
        socketfd = NETWORK_createConnectionClient(lseat.enterprise.port, ip);

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
                write(2, ERR_MEMORY, strlen(ERR_MEMORY));
            }


        }
    } else {
        write(1, ERR_DATA, strlen(ERR_DATA));
    }

    if (ip != NULL)free(ip);
    if (port != NULL)free(port);

    return socketfd;
}

int CONNECTION_analyseDataPacket(Packet packet) {

    if (packet.type == '1' && packet.length <= 0) {

        //If there is any error during the connection then we will recive a length of 0
        write(1, CONNECTION_NDATA, strlen(CONNECTION_NDATA) * sizeof(char));

        return 0;

    } else {

        write(1, CONNECTION_DATA, strlen(CONNECTION_DATA) * sizeof(char));

    }

    return 1;

}

void CONNECTION_requestMenuEnterprise() {
    Packet packet, packet1;
    char *name = NULL, *price = NULL, *units = NULL, *cadena = NULL;

    if (socketfd > 2) {
        packet = NETWORK_createPacket(MENU, MENU_PICENT, 0, NULL);

        if (NETWORK_openedSocket(socketfd) >0 && NETWORK_sendSerialized(socketfd, packet) > 0) {
            NETWORK_freePacket(&packet);
            packet1 = NETWORK_extractIncomingFrame(socketfd);

            if (packet1.type != 0) {
                write(1, "MENU DISPONIBLE\n", strlen("MENU DISPONIBLE\n") * sizeof(char));

                while (strcmp(packet1.header, MENU_END) != 0) {
                    UTILS_extractFromBuffer(packet1.data, 3, &name, &price, &units);

                    name[0] = name[0] - ('a' - 'A');
                    cadena = (char *) malloc(sizeof(char) * (strlen(name) + strlen(price) + 11));

                    sprintf(cadena, "%s (%s euros)\n", name, price);
                    write(1, cadena, strlen(cadena) * sizeof(char));

                    if (cadena != NULL) {
                        free(cadena);
                        cadena = NULL;
                    }

                    NETWORK_freePacket(&packet1);
                    packet1 = NETWORK_extractIncomingFrame(socketfd);
                }
            } else {
                write(1, "Error con los platos recividos\n", strlen("Error con los platos recividos\n") * sizeof(char));
            }
        } else {
            CONNECTION_enterpriseReconnect();

        }
    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));

    }

}

void CONNECTION_deleteDishMenu(char **data) {
    int num = 0, unitats = 0;
    char *plate = NULL;

    if (socketfd > 2) {

        plate = UTILS_toLower(data[1]);

        num = MSTRUCTURE_findElement(lseat.commands, plate);

        if (num >= 0) {
            unitats = atoi(data[0]);

            if (unitats <= lseat.commands.bucket[num].number && unitats > 0) {

                char *buffer = UTILS_createBuffer(2, data[1], data[0]);

                if (buffer != NULL) {

                    Packet packet = NETWORK_createPacket(DEL_DISH, DEL_ORD, (unsigned short) strlen(buffer), buffer);
                    if (NETWORK_openedSocket(socketfd) > 0 && NETWORK_sendSerialized(socketfd, packet) > 0) {
                        NETWORK_freePacket(&packet);

                        if (NETWORK_sendSerialized(socketfd, packet) < 0) {

                            close(socketfd);
                            write(1, ERR_CONN, strlen(ERR_CONN));
                            socketfd = -1;

                        }

                        if (NETWORK_readSimpleResponse(socketfd) > 0) {
                            write(1, "OK!\n", strlen("OK!\n") * sizeof(char));
                        } else {
                            write(1, NO_DPLAT, strlen(NO_DPLAT));
                        }

                    } else {
                        CONNECTION_enterpriseReconnect();
                    }
                    free(buffer);

                } else {

                    write(1, ERR_CONN, strlen(ERR_CONN));

                }

            } else {
                write(1,
                      "ERROR: nombre d'unitats incorrecte\n",
                      strlen("ERROR: nombre d'unitats incorrecte\n") * sizeof(char));
            }

        } else {
            write(1,
                  "ERROR: aquest plat no l'has demanat\n",
                  strlen("ERROR: aquest plat no l'has demanat\n") * sizeof(char));
        }
		if (data[0] != NULL)free(data[0]);
		if (data[1] != NULL)free(data[1]);
		free(data);

    } else {

        write(1, ERR_CONN, strlen(ERR_CONN));

    }
}

void CONNECTION_payEnterprise() {

    if (socketfd > 2) {

        Packet packet = NETWORK_createPacket(PAY, PAY_HEADER, (unsigned short) 0, NULL);

        if (NETWORK_openedSocket(socketfd) > 0 && NETWORK_sendSerialized(socketfd, packet) > 0) {

            NETWORK_freePacket(&packet);

            packet = NETWORK_extractIncomingFrame(socketfd);

            lseat.client.saldo -= atoi(packet.data);

            write(1, "Son ", strlen("Son ") * sizeof(char));
            write(1, packet.data, strlen(packet.data) * sizeof(char));
            write(1, " euros. Carregat en el seu compte.\n",
                  strlen(" euros. Carregat en el seu compte.\n") * sizeof(char));

            if (MSTRUCTURE_isEmpty(lseat.commands) > 0) {
				MSTRUCTURE_destruct(&lseat.commands);
            }
        } else {
            CONNECTION_enterpriseReconnect();

        }
    } else {

        write(1, ERR_CONN, strlen(ERR_CONN));

    }
}

void CONNECTION_takeNoteEnterprise(char **data) {
    char *buffer = UTILS_createBuffer(2, data[1], data[0]);

    if (socketfd > 2) {
        int trobat = 0;

        Packet packet = NETWORK_createPacket(DISH, NEW_ORD, (unsigned short) strlen(buffer), buffer);

        if ( NETWORK_openedSocket(socketfd) < 0 || NETWORK_sendSerialized(socketfd, packet) < 0 ) {

            CONNECTION_enterpriseReconnect();

        } else {

            if (NETWORK_readSimpleResponse(socketfd) > 0) {
                write(1, "OK!\n", strlen("OK!\n") * sizeof(char));

                data[1] = UTILS_toLower(data[1]);

				if (lseat.commands.bucket == NULL){
					lseat.commands = MSTRUCTURE_createStructure(MENU_SIZE);
					trobat = -1;
				}else{
					trobat = MSTRUCTURE_findElement(lseat.commands, data[1]);
				}

                if (trobat >= 0) {
                    MSTRUCTURE_incrementNum(&lseat.commands, trobat, atoi(data[0]));
                } else {
                    MSTRUCTURE_insert(&lseat.commands, MSTRUCTURE_createBucket(data[1], trobat, atoi(data[0])));
                }

            } else {
                write(1, NO_PLAT, strlen(NO_PLAT) * sizeof(char));
            }


            NETWORK_freePacket(&packet);

        }


    } else {

        write(1, ERR_CONN, strlen(ERR_CONN));

    }

    if (buffer != NULL)free(buffer);
    if (data[0] != NULL)free(data[0]);
    if (data[1] != NULL)free(data[1]);
    free(data);


}

void CONNECTION_disconnectEnterprise(char *nom) {
    if (socketfd > 2) {
        Packet packet = NETWORK_createPacket(DISCONNECT, HEADER_PICDAT, (unsigned short) strlen(nom), nom);

        if (NETWORK_sendSerialized(socketfd, packet) > 0) {

            if (NETWORK_readSimpleResponse(socketfd) > 0) {

                write(1, DESCONNECTING_OK, strlen(DESCONNECTING_OK));
                close(socketfd);

                socketfd = -1;

				if (MSTRUCTURE_isEmpty(lseat.commands) > 0) {
					MSTRUCTURE_destruct(&lseat.commands);
				}
            } else {
                write(1, CONNECTION_NENT, strlen(CONNECTION_NENT));

            }
        } else {
            CONNECTION_enterpriseReconnect();
        }
		NETWORK_freePacket(&packet);

    } else {
        write(1, ERR_CONN, strlen(ERR_CONN));
    }
}

void CONNECTION_enterpriseReconnect(){
    char *enterpriseData = NULL;

    write(1, "Enterprise ha caido\n", strlen("Enterprise ha caido\n") * sizeof(char));
    close(socketfd);
    socketfd = -1;
    enterpriseData = CONNECTION_data(2, lseat.config.Port, lseat.config.IP, lseat.client.nom);

    if (enterpriseData != NULL) {

        socketfd = CONNECTION_enterprise(enterpriseData, lseat.client.nom, lseat.client.saldo);
        if(NETWORK_openedSocket(socketfd) > 0 && socketfd > 0)CONNECTION_resendCommands(socketfd,&lseat.commands);

    }else{
        lseat.enterprise.port = -1;
        write(1,"Error en conectarnos a Data!\n", strlen("Error en conectarnos a Data!\n") * sizeof(char));
    }
}


void CONNECTION_resendCommands(int socket,Menu *table){
    int i=0;
    char *buffer = NULL, *cadena = NULL;
    Packet packet;

    write(1, "Reenviando Pedidos!\n", strlen("Reenviando Pedidos!\n") * sizeof(char));
    for(i=0; i < table->length; i++){

        if(table->bucket[i].key != NULL){

            cadena = (char *) malloc (sizeof(char) * 10);

            sprintf(cadena,"%d",table->bucket[i].number);

            buffer = UTILS_createBuffer(2,table->bucket[i].key,cadena);

            packet = NETWORK_createPacket(DISH, NEW_ORD, (unsigned short) strlen(buffer), buffer);

			free(buffer);
			free(cadena);

            if ( NETWORK_openedSocket(socket) < 0 || NETWORK_sendSerialized(socket, packet) < 0 ) {

				lseat.enterprise.port = -1;
                write(1,"Error en conectarnos a un nuevo Enterprise\n", strlen("Error en conectarnos a un nuevo Enterprise\n")* sizeof(char));
            } else {

                if (NETWORK_readSimpleResponse(socketfd) > 0) {

                    write(1,"Plato ", strlen("Plato ") * sizeof(char));
                    write(1,table->bucket[i].key, strlen(table->bucket[i].key) * sizeof(char));
                    write(1, " se ha podido anotar\n", strlen(" se ha podido anotar\n") * sizeof(char));

                } else {

                    write(1,"Plato ", strlen("Plato ") * sizeof(char));
                    write(1,table->bucket[i].key, strlen(table->bucket[i].key) * sizeof(char));
                    write(1, " NO se ha podido anotar\n", strlen(" NO se ha podido anotar\n") * sizeof(char));

                    MSTRUCTURE_delete(table,table->bucket[i].key);

                }


                NETWORK_freePacket(&packet);

            }

        }
    }
}
