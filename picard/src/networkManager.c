//
// Created by gabriel on 31/10/17.
//

#include "../include/picardController.h"


int connectToData(ClientLSEat *lsEat){
	int socket = 0, error = 0;

	write(1, CONNECTING, strlen(CONNECTING));
	socket = createConnectionClient(lsEat->config.Port,lsEat->config.IP);
	if(socket < 0){
		write(1,ERR_CONN,strlen(ERR_CONN));
		exit(EXIT_FAILURE);
	}

	return socket;
}
/**
 * Function to fill buffer with more data
 * @param buffer
 * @param info
 */
void fillBuffer(char** buffer, char* info){
	printf("BUFF: %s\n",*buffer);
	printf("INFO: %s\n",info);
	*buffer = (char*) realloc(*buffer, sizeof(info));
	//strcpy(*buffer,info);
	strcat(*buffer,info);
}
int sendInfoData(int socket,ClientLSEat lsEat){
	char *buffer = NULL;
	char cadena[2];
	int size = (int)(strlen(lsEat.client.nom));

	sprintf(cadena, "%d",size);

//	printf("Size: %s\n",(cadena));
	fillBuffer(&buffer,PIC_NAME);
//	printf(" TYPE: %s\n",buffer);
	fillBuffer(&buffer,"PIC_NAME\0\0");
//	printf(" TYPE+header: %s\n",buffer);
	fillBuffer(&buffer,cadena);
//	printf(" TYPE+header+length: %s\n",buffer);
	fillBuffer(&buffer,lsEat.client.nom);
//	printf(" TYPE+header+length+data: %s\n",buffer);
	write(socket, buffer,strlen(buffer));
	return 0;
}
