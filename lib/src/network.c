
#include "../include/network.h"


int NETWORK_createConnectionClient(int portInput, char *ipInput) {

	// comprovem la validesa del port
	if (portInput < 1 || portInput > 65535) {
		fprintf(stderr, ERR_PORT, portInput);
		return -1;
	}
	uint16_t port;
	port = portInput;
	// comprovem la validesa de l'adre￿a ip
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

int NETWORK_createConnectionServer(int portInput, char *ipInput) {

	int retVal;

	// we check if the port is alright
	uint16_t port;
	if (portInput < 1 || portInput > 65535) {
		fprintf(stderr, ERR_PORT, portInput);
		return -1;
	}
	port = portInput;

	struct in_addr ip_addr;
	if (inet_aton(ipInput, &ip_addr) != 1) {
		fprintf(stderr, ERR_ATON, ipInput, strerror(errno));
		return -1;
	}

	// now its time to create the socket
	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket TCP");
		return -1;
	}

	struct sockaddr_in s_addr;
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = inet_addr(ipInput);//INADDR_ANY;


	retVal = bind(sockfd, (void *) &s_addr, sizeof(s_addr));
	if (retVal < 0) {
		perror("bind");
		return -1;
	}

	listen(sockfd, 64);
	return sockfd;
}

void NETWORK_serialHandler(int socketfd, void *(*handler)(void *)) {

	struct sockaddr_in c_addr;
	socklen_t c_len = sizeof(c_addr);

	if (socketfd < 0) {
		exit(EXIT_FAILURE);
	}

	while (1) {

		int newsock = accept(socketfd, (void *) &c_addr, &c_len);

		if (newsock < 0) {
			perror("accept");
			close(socketfd);
			break;
		}

		handler((void *) &newsock);
	}
}

void NETWORK_parallelHandler(int port, char *ip, void *(*handler)(void *)) {

	int sockfd;
	if (ip != NULL) {
		sockfd = NETWORK_createConnectionServer(port, ip);
	} else {
		sockfd = port;
	}

	pthread_t thread_id;

	while (1) {
		struct sockaddr_in c_addr;
		socklen_t c_len = sizeof(c_addr);

		int newsock = accept(sockfd, (void *) &c_addr, &c_len);
		if (newsock < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		if (pthread_create(&thread_id, NULL, handler, &newsock) < 0) {
			perror("could not create thread");
			close(newsock);
		}
	}
}

int NETWORK_sendSerialized(int socket, Packet packet) {
	char *buffer = NULL;
	unsigned short size;

	if( packet.data != NULL){

		size = (unsigned short) (SIMPLE_PACKET_LENGTH + strlen(packet.data)) ;

	}else{

		size = (unsigned short) (SIMPLE_PACKET_LENGTH) ;

	}

	buffer = (char *) calloc(size,sizeof(char) * size);

	if (buffer == NULL) {
		return -1;
	}


	buffer[0] = packet.type;

	memcpy(buffer + sizeof(char), packet.header, HEADER_SIZE);

	buffer[HEADER_SIZE + TYPE_SIZE] = (char) packet.length;

	if(packet.data != NULL)memcpy(buffer + SIMPLE_PACKET_LENGTH, packet.data, packet.length);

	if (write(socket, buffer, size) == size) {

		free(buffer);

		return 1;
	}

	free(buffer);

	return -1;

}

Packet NETWORK_createPacket(int type, char *header, int length, char *data) {

	Packet packet;
	packet.type = 0;
	packet.data = NULL;
	packet.length = 0;

	memset(packet.header, '\0', sizeof(char) * HEADER_SIZE);

	sprintf(&packet.type, "%d", type);

	memcpy(packet.header, header, HEADER_SIZE);
	packet.length = (unsigned short) length;

	if (length > 0) {

		packet.data = (char *) calloc(packet.length + 1, sizeof(char) * (packet.length + 1));

		if (packet.data == NULL) {

			packet.type = ERROR_CODE;
			return packet;

		}

		memcpy(packet.data, data, packet.length);

	}

	return packet;

}

int NETWORK_readSimpleResponse(int socket) {

	char header[HEADER_SIZE], type;

	read(socket, &type, sizeof(char));
	read(socket, header, HEADER_SIZE * sizeof(char));

	switch (type) {
		case '1':
			return (strcmp(header, HEADER_CON) == 0);
		case '2':
			return (strcmp(header, HEADER_ENDMENU) == 0);
		case '3':
			return (strcmp(header, HEADER_ORD) == 0);
		case '5':
			return (strcmp(header, HEADER_ORD) == 0);
		case '6':
			return (strcmp(header, HEADER_PAY) == 0);
		case '7':
			return (strcmp(header, HEADER_NUPDATE) == 0);
		default:
			break;
	}

	return 1;

}

Packet NETWORK_extractIncomingFrame(int socket) {

	char type, header[HEADER_SIZE], *data = NULL;
	unsigned short length = 0;
	Packet packet;

	memset(header, '\0', HEADER_SIZE);

	if (read(socket, &type, sizeof(char)) < 0) {
		close(socket);
		packet.type = ERROR_CODE;
		return packet;
	}

	if (read(socket, &header, sizeof(char) * HEADER_SIZE) < 0) {
		close(socket);
		packet.type = ERROR_CODE;
		return packet;
	}

	if (!strcmp(header, HEADER_NCON)) {

		close(socket);

	} else {

		if (read(socket, &length, sizeof(unsigned short)) < 0) {

			close(socket);
			packet.type = ERROR_CODE;
			return packet;

		}


		if (length > 0) {

			data = (char *) calloc(length,sizeof(char) * length);

			if (data != NULL) {

				if (read(socket, data, sizeof(char) * length) > 0) {

					packet = NETWORK_createPacket(type - '0', header, length, data);

					free(data);

					return packet;

				} else {

					free(data);
					close(socket);

				}

			}else{

				close(socket);

			}
		} else {

			return NETWORK_createPacket(type - '0', header, length, "\0");

		}
	}

	packet.type = ERROR_CODE;
	return packet;

}

void NETWORK_sendKOPacket(int socket, int type, char* header) {
	Packet aux;
	//if not we send the KO packet to the client
	aux = NETWORK_createPacket(type, header, (unsigned short) 0, NULL);
	NETWORK_sendSerialized(socket, aux);
	NETWORK_freePacket(&aux);

}

void NETWORK_sendOKPacket(int socket, int type, char* header) {
	Packet aux;

	aux = NETWORK_createPacket(type, header, (unsigned short) 0, NULL);
	NETWORK_sendSerialized(socket, aux);
	NETWORK_freePacket(&aux);

}

void NETWORK_freePacket(Packet *packet){

	if(packet->data != NULL){
		free(packet->data);
		packet->data = NULL;
	}
	packet->type = 0;
	packet->length = 0;
}