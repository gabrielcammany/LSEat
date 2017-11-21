#include <memory.h>
#include <errno.h>
#include "../include/network.h"

int createConnectionClient(int portInput, char *ipInput) {

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

int createConnectionServer(int portInput, char *ipInput) {

	int retVal;

	// comprovem la validesa del port
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

	// creem el socket
	int sockfd;
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
		close(sockfd);
		return -1;
	}

	listen(sockfd, 64);
	return sockfd;
}

void serialHandler(int socketfd, void *(*handler)(void *)) {

	struct sockaddr_in c_addr;
	socklen_t c_len = sizeof(c_addr);
	printf("Socket: %d\n", socketfd);

	if (socketfd < 0) {
		exit(EXIT_FAILURE);
	}

	while (1) {

		write(1, WAIT_CLIENT, strlen(WAIT_CLIENT));

		int newsock = accept(socketfd, (void *) &c_addr, &c_len);
		printf("Close 5: %d\n", newsock);
		if (newsock < 0) {
			perror("accept");
			printf("Close 4: %d\n", socketfd);
			close(socketfd);
			exit(EXIT_FAILURE);
		}


		handler((void *) &newsock);
	}
}

void parallelHandler(int port, char *ip, void *(*handler)(void *), void *arg) {
	char *buff = NULL;
	int sockfd;

	if(ip != NULL){
		sockfd = createConnectionServer(port, ip);
	}else{
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

		buff = (char *) malloc(sizeof(char) * strlen(MSG_NEW) + 25);
		sprintf(buff, MSG_NEW, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
		write(1, buff, strlen(buff));


		if (pthread_create(&thread_id, NULL, handler, arg) < 0) {
			perror("could not create thread");
			close(newsock);
		}
	}
}

int sendSerialized(int socket, Packet packet) {
	char *buffer;

	unsigned short size = (unsigned short) (SIMPLE_PACKET_LENGTH + strlen(packet.data));

	buffer = (char *) malloc(sizeof(char) * size);
	if (buffer == NULL) {
		return 0;
	}

	memset(buffer, 0, size);

	if (packet.data != NULL) {

		buffer[0] = packet.type;
		memcpy(buffer + sizeof(char), packet.header, HEADER_SIZE);

		buffer[HEADER_SIZE + TYPE_SIZE] = (char) (packet.length & 0x0F);
		buffer[HEADER_SIZE + TYPE_SIZE + 1] = (char) ((packet.length & 0xF0) >> 8);

		memcpy(buffer+SIMPLE_PACKET_LENGTH,packet.data,packet.length);

		if (write(socket, buffer, size) == size) {

			free(buffer);
			return 1;
		}

	}


	free(buffer);
	return 0;

}

Packet createPacket(char type, char *header, unsigned short length, char *data) {

	Packet packet;

	memset(packet.header, '\0', sizeof(char) * HEADER_SIZE);

	packet.type = type;

	memcpy(packet.header, header,HEADER_SIZE);

	packet.length = length;

	if (data != NULL && length > 0) {

		packet.data = (char *) malloc(sizeof(char) * strlen(data));

		if (packet.data == NULL) {

			packet.type = 0;
			return packet;

		}

		memset(packet.data, 0, sizeof(char) * packet.length);

		memcpy(packet.data, data, packet.length);

	} else {
		packet.type = 0;
	}


	return packet;

}

int readSimpleResponse(int socket) {

	char header[HEADER_SIZE], type;

	read(socket, &type, sizeof(char));

	switch (type) {
		case CONNECT || DISCONNECT:
			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_CON) == 0);
		case MENU:

			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_ENDMENU) == 0);
		case DISH:

			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_ORD) == 0);
		case DEL_DISH:

			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_ORD) == 0);
		case PAY:

			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_PAY) == 0);
		case UPDATE:

			read(socket, header, HEADER_SIZE * sizeof(char));

			return (strcmp(header, HEADER_NUPDATE) == 0);
		default:
			break;
	}
	return 0;

}

Packet extractIncomingFrame(int socket) {

	char type, header[HEADER_SIZE], *data = NULL;
	unsigned short length = 0;
	Packet packet;

	memset(header,'\0',HEADER_SIZE);

	read(socket, &type, sizeof(char));

	read(socket, &header, sizeof(char) * HEADER_SIZE);


	if (!strcmp(header, HEADER_NCON)) {

		close(socket);

	} else {

		read(socket, &length, sizeof(unsigned short));

		data = (char *) malloc(sizeof(char) * length);

		if (data != NULL) {

			if (read(socket, data, sizeof(char) * length) > 0) {


				return createPacket(type, header, length, data);

			}

		}

	}
	packet.type = 0;
	return packet;

}