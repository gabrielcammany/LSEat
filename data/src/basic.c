//
// Created by gabriel on 19/11/17.
//

#include "../include/controller.h"

void basic_freeMemory(Data *data) {
	free(data->ip);
}

void basic_startValues(Data *data) {
	data->ip = NULL;
	data->enterprisePort = 10;
	data->picardPort = 10;
	socketPic = 5;
	socketEnt = 6;
}


/**
 * Function to get ports from files
 * @param fd file descriptor from  where to read
 * @param port port readed
 * @return
 */
int basic_readPorts(int fd, int *portNumber){
	int error = 0;
	char *port = NULL;

	//Now it's time for the picard port
	error = readDynamic(&port, fd);

	//Two kinds of errors
	if (!error) {
		write(1, ERR_PORTP, strlen(ERR_PORTP));
		close(fd);
		return EXIT_FAILURE;
	} else if (error < 0) {
		write(1, ERR_MEMORY, strlen(ERR_MEMORY));
		close(fd);
		return EXIT_FAILURE;
	}

	*portNumber = atoi(port);
	return EXIT_SUCCESS;

}

int basic_readDataConfig(char* name,Data *data) {

	int fd = 0;
	int error;

	fd = openFile(name, 1);
	if (fd == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	//We read Data ip from config file
	error = readDynamic(&data->ip, fd);
	if (error < 0) {
		write(1, ERR_IP, strlen(ERR_IP));
		close(fd);
		return EXIT_FAILURE;
	}
	if ( basic_readPorts(fd,&data->picardPort) != EXIT_FAILURE ) {
		error = basic_readPorts(fd,&data->enterprisePort);
	}


	close(fd);

	return error;
}