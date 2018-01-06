/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: socketUtils.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef socketUtils_H
#define socketUtils_H

//include system
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>

//own includes
#include "utils.h"
#include "../include/llista.h"

//constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"


#define WAIT_CLIENT "Esperant clients...\n"

#define HEADER_SIZE 10
#define LENGTH_SIZE 2
#define TYPE_SIZE 1

#define SIMPLE_PACKET_LENGTH (HEADER_SIZE+LENGTH_SIZE+TYPE_SIZE)

/**
 * Types
 */

#define CONNECT 1
#define DISCONNECT 2
#define MENU 3
#define DISH 4
#define DEL_DISH 5
#define PAY 6
#define UPDATE 7
#define RECONNECT 8
/**
 * HEADERS
 */

#define HEADER_PICINF "[PIC_INF]"
#define HEADER_PICDAT "[PIC_NAME]"
#define HEADER_DATPIC "[ENT_INF]"

#define MENU_PICENT "[SHW_MENU]"
#define END_MENU "[END_MENU]"
#define MENU_DISH "[DISH]"
#define MENU_END "[END_MENU]"

#define DEL_ORD "[DEL_ORD]"
#define NEW_ORD "[NEW_ORD]"
#define PAY_HEADER "[PAY]"

#define HEADER_CON "[CONOK]"
#define HEADER_NCON "[CONKO]"

#define HEADER_ORD "[ORDOK]"
#define HEADER_NORD "[ORDKO]"

#define HEADER_PAY "[PAYOK]"
#define HEADER_NPAY "[PAYKO]"

#define HEADER_ENDMENU "[END_MENU]"

#define HEADER_UPDATE "[UPDATEOK]"
#define HEADER_NUPDATE "[UPDATEKO]"
#define HEADER_EUPDATE "[UPDATE]"


/**
 * Structure where we are going to save data
 * to be able to comunicate between servers and clients
 */
typedef struct {
	char type;
	char header[HEADER_SIZE];
	unsigned short length;
	char *data;
} Packet;


/**
            - Client functions -
**/

int NETWORK_createConnectionClient(int portInput, char *ipInput);


/**
            - Server functions -
**/

int NETWORK_createConnectionServer(int portInput, char *ipInput);

void NETWORK_serialHandler(int socketfd, void *(*handler)(void *));

void NETWORK_parallelHandler(int port, char *ip, void *(*handler)(void *)/*, void *arg*/);

/**
 * Sned serialized packet to server
 * @param socket file descriptor of connection
 * @param packet information structure
 * @return
 */
int NETWORK_sendSerialized(int socket, Packet packet);

/**
 * Puts all information in packet structure
 * @param type id of packet
 * @param header name of packet
 * @param length size of data
 * @param data information
 * @return packet structure filled
 */
Packet NETWORK_createPacket(char type, char *header, int length, char *data);

int NETWORK_readSimpleResponse(int socket);

Packet NETWORK_extractIncomingFrame(int socket);

void NETWORK_sendKOPacket(int socket, int type, char* header);

void NETWORK_sendOKPacket(int socket, int type, char* header);

void NETWORK_freePacket(Packet *packet);

void NETWORK_printPacket(Packet packet);

int NETWORK_openedSocket(int socket);
#endif
