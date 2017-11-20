//
// Created by gabriel on 19/11/17.
//

#include "../include/controller.h"

void basic_freeMemory(Data *data) {
	free(data->IP);
	free(data->enterprisePort);
	free(data->picardPort);
}

void basic_startValues(Data *data) {
	data->IP = NULL;
	data->enterprisePort = NULL;
	data->picardPort = NULL;
}


/**
 * Function to get ports from files
 * @param fd file descriptor from  where to read
 * @param port port readed
 * @return
 */
int basic_readPorts(int fd, char **port){
	int error = 0;
	*port = NULL;

	//Now it's time for the picard port
	error = readDynamic(port, fd);

	//Two kinds of errors
	if (!error) {
		write(1, ERR_PORTP, strlen(ERR_PORTP));
		close(fd);
		error = ERROR_CODE;
	} else if (error < 0) {
		write(1, ERR_MEMORY, strlen(ERR_MEMORY));
		close(fd);
		error = ERROR_CODE;
	}

	return error;
}

int basic_readDataConfig(char* name,Data *data) {

	int fd = 0;
	int error;

	fd = openFile(name, 1);
	if (fd < 0) {
		return ERROR_CODE;
	}
	//We read Data ip from config file
	error = readDynamic(&data->IP, fd);
	if (error < 0) {
		write(1, ERR_IP, strlen(ERR_IP));
		close(fd);
		error = ERROR_CODE;
	}

	if (error > 0) {
		error = basic_readPorts(fd,&data->picardPort);
		if ( error > 0 ) {
			error = basic_readPorts(fd,&data->enterprisePort);

		}
	}

	close(fd);

	return error;
}