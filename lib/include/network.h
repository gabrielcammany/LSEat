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

//include propios
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "utils.h"

//constantes propias
#define ERR_PORT "Error: %d es un port invalid\n"
#define ERR_ATON "inet_aton (%s): %s\n"
#define MSG_NEW "Nova connexio de %s:%hu\n"


#define WAIT_CLIENT "Esperant clients...\n"

#define HEADER_SIZE 10
#define LENGTH_SIZE 2
#define TYPE_SIZE 1

#define SIMPLE_PACKET_LENGTH (HEADER_SIZE+LENGTH_SIZE+TYPE_SIZE)

typedef struct {
	char type;
	char header[HEADER_SIZE];
	char length[LENGTH_SIZE];
	char *data;
} Packet;

typedef struct {
	int socket;
	void* data;
} Forward;

#define CONNECTION_KO 1,"[CONKO]",0,"\0"
#define DISCONNECT_KO 1,"[CONKO]",0,"\0"

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


/**
 * HEADERS
 */

#define HEADER_CON "[CONOK]"
#define HEADER_NCON "[CONOK]"

#define HEADER_PICINF "[PIC_INF]"


#define HEADER_ORD "[ORDOK]"
#define HEADER_NORD "[ORDKO]"

#define HEADER_PAY "[PÀYOK]"
#define HEADER_NPAY "[PAYKO]"

#define HEADER_ENDMENU "[END_MENU]"

#define HEADER_UPDATE "[UPDATEOK]"
#define HEADER_NUPDATE "[UPDATEKO]"


/**
 * DATA
 */
#define CONNECTION_DATPIC 1,"[ENT_INF]"

/**
 * ENTERPRISE
 */
#define CONNECTION_ENTPIC 1,"[CONOK]",0,"\0"

#define DISCONNECT_ENTPIC 2,"[CONOK]",0,"\0"

#define MENU_ENTPIC 3,"[DISH]"
#define END_MENU_ENTPIC 3,"[END_MENU]",0,"\0"

#define DISH_ENTPIC 4,"[ORDOK]",0,"\0"
#define NO_DISH_ENTPIC 4,"[ORDKO]",0,"\0"

#define DELDISH_ENTPIC 5,"[ORDOK]",0,"\0"
#define NO_DELDISH_ENTPIC 5,"[ORDKO]",0,"\0"

#define PAY_ENTPIC 6,"[PAYOK]"
#define NO_PAY_ENTPIC 6,"[PAYKO]",0,"\0"


/**
            - Client functions -
**/

int createConnectionClient(int portInput, char *ipInput);


/**
            - Server functions -
**/

int createConnectionServer(int portInput, char *ipInput);

int serialHandler(char* port, char* ip, void *(*handler)(void*));

void parallelHandler(char *port, char *ip, void *(*handler)(void *), void *arg);

int sendSerialized(int socket, Packet packet);

Packet createPacket(char type, char *header, int length, char *data);

int readSimpleResponse(int socket);

Packet extractIncomingFrame(int socket);

#endif