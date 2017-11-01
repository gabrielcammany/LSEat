/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 26-10-2017
 */

#include "files.h"

int openFile(char *name, int status) {
	int fd = 0, empty;
	char c;

	switch (status) {
		case 1:
			fd = open(name, O_RDONLY);
			break;
		case 2:
			fd = open(name, O_RDWR);
			break;
		default:
			write(1, ERR_ARGUMENT, strlen(ERR_ARGUMENT));
			return -1;
	}

	if (fd < 0) {
		write(1, ERR_OP_FILE, strlen(ERR_OP_FILE));
		return -1;
	}
	empty = read(fd, &c, 1);
	if (empty == 0) {
		write(1, ERR_EMPTY_FILE, strlen(ERR_EMPTY_FILE));
		return 0;
	}
	lseek(fd, 0, SEEK_SET);
	return fd;
}

/*
int createFile(char *nom, char *permissions){
   return 0;
}
*/

int readNetworkConfig(int fd, Config *config) {
	char *cadena = NULL;
	int error = 0;


	error = readDynamic(&config->IP, fd);
	if (!error) {
		write(1, ERR_IP_FILE, strlen(ERR_IP_FILE));
		close(fd);
		error = -1;
	} else if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}
	if (error != -1) {
		error = readDynamic(&cadena, fd);
		if (!error) {
			write(1, ERR_PORT_FILE, strlen(ERR_PORT_FILE));
			close(fd);
		} else if (error < 0) {
			write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
			close(fd);
		} else {
			config->Port = atoi(cadena);
			if (config->Port == 0) {
				write(1, ERR_FORMAT_PORT_FILE, strlen(ERR_FORMAT_PORT_FILE));
				close(fd);
			} else {
				error = 0;
			}
		}
	}

	if (cadena != NULL) {
		free(cadena);
	}
	return 0;
}

int readClientConfig(char *name, ClientLSEat *lseat) {

	int fd = 0;
	char *cadena = NULL;
	int error;

	lseat->config.IP = NULL;
	lseat->client.nom = NULL;


	fd = openFile(name, 1);
	if (fd < 0) {
		return -1;
	}

	error = readDynamic(&lseat->client.nom, fd);
	if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}

	if (error > 0) {
		//LLegim el saldo corresponent
		error = readDynamic(&cadena, fd);
		if (!error) {
			write(1, ERR_SALDO_FILE, strlen(ERR_SALDO_FILE));
			close(fd);
			error = -1;
		} else if (error < 0) {
			write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
			close(fd);
		} else {
			lseat->client.saldo = atoi(cadena);
			if (lseat->client.saldo == 0) {
				write(1, ERR_FORMAT_SALDO_FILE, strlen(ERR_FORMAT_SALDO_FILE));
				close(fd);
				error = -1;
			}
		}
	}

	if (error > 0) {
		error = readNetworkConfig(fd, &lseat->config);
	}

	close(fd);

	if (cadena != NULL) {
		free(cadena);
	}
	return error;
}
