#include <memory.h>
#include <errno.h>
#include "../include/network.h"

#define MSG "[Introduir missatge]"

int createConnectionClient(int portInput, char *ipInput) {

	// comprovem la validesa del port
	if (portInput < 1 || portInput > 65535) {
		fprintf(stderr, ERR_PORT, portInput);
		return -1;
	}
	uint16_t port;
	port = portInput;
	// comprovem la validesa de l'adre￿a IP
	// i la convertim a format binari
	struct in_addr ip_addr;
	if (inet_aton(ipInput, &ip_addr) == 0) {
		fprintf(stderr, ERR_ATON, ipInput, strerror(errno));
		return -1;
	}
	// creem el socket
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket TCP");
		return -1;
	}
	// especifiquem l'adre￿a del servidor
	struct sockaddr_in s_addr;
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr = ip_addr;

	if (connect(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
		perror("connect");
		return -1;
	}
	return sockfd;
}

int createConnectionServer(int portInput, char *ipInput) {

	// comprovem la validesa del port
	uint16_t port;
	if (portInput < 1 || portInput > 65535) {
		fprintf(stderr, ERR_PORT, portInput);
		return -1;
	}
	port = portInput;

	struct in_addr ip_addr;
	if (inet_aton(ipInput, &ip_addr) == 0) {
		fprintf(stderr, ERR_ATON, ipInput, strerror(errno));
		return -1;
	}

	// creem el socket
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket TCP");
		return -1;
	}
	// especifiquem l'adreca que volem vincular al nostre socket
	// admetrem connexions dirigides a qualsevol IP de la nostra m￿quina
	// al port especificat per linia de comandes
	struct sockaddr_in s_addr;
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr = ip_addr;//INADDR_ANY;
	// al cridar bind, hem de fer un cast:
	// bind espera struct sockaddr *
	// i nosaltres passem struct sockaddr_in *
	if (bind(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
		perror("bind");
		return -1;
	}
	// ara obrim el port, 5 es un valor tipic
	listen(sockfd, 64);
	return sockfd;
}

int serialHandler(char *port, char *ip, void *(*handler)(void *)) {
	int sockfd;
	sockfd = createConnectionServer(atoi(port), ip);

	if (sockfd < 0) {
		return EXIT_FAILURE;
	}

	write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));

	while (1) {
		struct sockaddr_in c_addr;
		socklen_t c_len = sizeof(c_addr);


		int newsock = accept(sockfd, (void *) &c_addr, &c_len);
		if (newsock < 0) {
			perror("accept");
			return EXIT_FAILURE;
		}

		handler((void *) &newsock);
	}
}

void parallelHandler(char *port, char *ip, void *(*handler)(void *), void *arg) {
	char *buff = NULL;
	int sockfd;
	sockfd = createConnectionServer(atoi(port), ip);
	pthread_t thread_id;

	while (1) {
		struct sockaddr_in c_addr;
		socklen_t c_len = sizeof(c_addr);

		int newsock = accept(sockfd, (void *) &c_addr, &c_len);
		if (newsock < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		buff = (char *) malloc(sizeof(char) * strlen(MSG_NEW) + 25);
		sprintf(buff, MSG_NEW, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
		write(1, buff, strlen(buff));


		if (pthread_create(&thread_id, NULL, handler, arg) < 0) {
			perror("could not create thread");
			close(newsock);
		}
	}
}

int sendSerialized(int socket, Packet packet){
	char* buffer;

	buffer = (char*) malloc(sizeof(char) * (SIMPLE_PACKET_LENGTH + strlen(packet.data)));
	if(buffer == NULL){
		return 0;
	}
	strcat(buffer,&packet.type);
	strcat(buffer,packet.header);
	strcat(buffer,packet.length);

	if(packet.data != NULL){
		strcat(buffer,packet.data);
	}

	if(write(socket,buffer,strlen(buffer)) == (int)strlen(buffer)){
		free(buffer);
		return 1;
	}

	free(buffer);
	return 0;

}

Packet createPacket(char type, char *header, int length, char *data){

	Packet packet;
	char auxLength[LENGTH_SIZE];
	memset(packet.header,'\0',sizeof(char)*HEADER_SIZE);

	packet.type = type;
	strcpy(packet.header, header);

	if(length > 0){
		sprintf(auxLength,"%d",length);
		strcpy(packet.length,auxLength);
	}else{
		memset(packet.length,0,sizeof(char)*LENGTH_SIZE);
	}

	if(data != NULL){
		packet.data = (char*)malloc(sizeof(char) * strlen(data));
		if(packet.data == NULL){
			packet.type = 0;
			return packet;
		}
		memset(packet.data,0,sizeof(char)*strlen(data));
		strcpy(packet.data,data);
	}else{
		packet.data = NULL;
	}



	return packet;

}

int readSimpleResponse(int socket){

	char header[HEADER_SIZE], type;

	read(socket,&type,sizeof(char));

	switch (type){
		case CONNECT || DISCONNECT:
			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_CON) == 0 ? 1 : 0);
		case MENU:

			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_ENDMENU) == 0 ? 1 : 0);
		case DISH:

			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_ORD) == 0 ? 1 : 0);
		case DEL_DISH:

			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_ORD) == 0 ? 1 : 0);
		case PAY:

			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_PAY) == 0 ? 1 : 0);
		case UPDATE:

			read(socket,header,HEADER_SIZE * sizeof(char));

			return (strcmp(header,HEADER_NUPDATE) == 0 ? 1 : 0);
		default:
			break;
	}
	return 0;

}

Packet extractIncomingFrame(int socket){

	char type, header[HEADER_SIZE], length[LENGTH_SIZE], *data = NULL;
	int convert = 0;
	Packet packet;

	read(socket, &type, sizeof(char));
	read(socket, &header, sizeof(char) * HEADER_SIZE);

	if (!strcmp(header, HEADER_NCON)) {

		close(socket);

	} else {
		read(socket, &length, sizeof(char) * LENGTH_SIZE);

		convert = atoi(length);
		if (convert > 0) {
			data = (char *) malloc(sizeof(char) * convert);
			if (data != NULL) {
				if(read(socket, data, sizeof(char) * convert) > 0){
					return createPacket(type,header,convert,data);
				}

			}
		}
	}
	packet.type = 0;
	return packet;

}