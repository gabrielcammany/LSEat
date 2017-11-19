//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"

void basic_startupMissages() {
	char cadena[100];
	//Message
	sprintf(cadena, WELCOME, lseat.client.nom);
	write(1, cadena, strlen(cadena));

	sprintf(cadena, SALDO, lseat.client.saldo);
	write(1, cadena, strlen(cadena));

	write(1, INTRODUCTION, strlen(INTRODUCTION));
}

void basic_startValues(Command *command) {
	lseat.client.nom = NULL;
	lseat.client.saldo = 0;
	lseat.config.IP = NULL;
	lseat.config.Port = 0;
	command->data = NULL;
	command->id = -1;
}

/**
 * Reads network configurations
 * @param fd File Descriptor from configuration file
 * @param config Structure where configuration is saved
 * @return
 */
int basic_readNetworkConfig(int fd, Config *config) {
	char *cadena = NULL;
	int error = 0;


	error = readDynamic(&config->IP, fd);
	if (!error) {
		write(1, ERR_IP_FILE, strlen(ERR_IP_FILE));
		close(fd);
		error = ERROR_CODE;
	} else if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}
	if (error != ERROR_CODE) {
		error = readDynamic(&cadena, fd);
		if (!error) {
			write(1, ERR_PORT_FILE, strlen(ERR_PORT_FILE));
			error = ERROR_CODE;
			close(fd);
		} else if (error < 0) {
			write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
			close(fd);
		} else {
			config->Port = atoi(cadena);
			if (config->Port == 0) {
				write(1, ERR_FORMAT_PORT_FILE, strlen(ERR_FORMAT_PORT_FILE));
				error = ERROR_CODE;
				close(fd);
			} else {
				error = 0;
			}
		}
	}

	if (cadena != NULL) {
		free(cadena);
	}
	return error;
}

int basic_readClientConfig(char *name, ClientLSEat *lseat) {

	int fd = 0;
	char *cadena = NULL;
	int error;

	lseat->config.IP = NULL;
	lseat->client.nom = NULL;


	fd = openFile(name, 1);
	if (fd < 0) {
		return ERROR_CODE;
	}
	//We read the clients name through the function read Dynamic
	//Which reads dynamically controlling memory usage
	error = readDynamic(&lseat->client.nom, fd);
	if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}

	if (error > 0) {
		//Now it's time for the money
		error = readDynamic(&cadena, fd);

		//Two kinds of errors
		if (!error) {
			write(1, ERR_SALDO_FILE, strlen(ERR_SALDO_FILE));
			close(fd);
			error = ERROR_CODE;
		} else if (error < 0) {
			write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
			close(fd);
		} else {

			//If everything is fine we make atoi (ascii -> int)
			lseat->client.saldo = atoi(cadena);
			if (lseat->client.saldo == 0) {
				write(1, ERR_FORMAT_SALDO_FILE, strlen(ERR_FORMAT_SALDO_FILE));
				close(fd);
				error = ERROR_CODE;
			}
		}
	}

	//Now it's time for the network configuration (IP and port)
	if (error > 0) {
		error = basic_readNetworkConfig(fd, &lseat->config);
	}

	close(fd);

	if (cadena != NULL) {
		free(cadena);
	}
	return error;
}

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void basic_freeMemory() {

	free(lseat.client.nom);
	free(lseat.config.IP);

}
