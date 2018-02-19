//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"

void BASIC_startupMessages() {
	char cadena[100];
	//Message
	sprintf(cadena, WELCOME, lseat.client.nom);
	write(1, cadena, strlen(cadena));

	sprintf(cadena, SALDO, lseat.client.saldo);
	write(1, cadena, strlen(cadena));

	write(1, INTRODUCTION, strlen(INTRODUCTION));
}

void BASIC_startValues(Command *command) {
	lseat.client.nom = NULL;
	lseat.client.saldo = 0;
	lseat.config.IP = NULL;
	lseat.config.Port = 0;
	lseat.enterprise.name = NULL;
	lseat.enterprise.port = 0;
	lseat.enterprise.ip = NULL;
	command->data = NULL;
	command->id = -1;
	socketfd = -1;
	lseat.commands = MSTRUCTURE_createStructure(MENU_SIZE);
}

/**
 * Reads network configurations
 * @param fd File Descriptor from configuration file
 * @param config Structure where configuration is saved
 * @return
 */
int BASIC_readNetworkConfig(int fd, Config *config) {
	char *cadena = NULL;
	int error = 0;


	error = UTILS_readDynamic(&config->IP, fd);
	if (!error) {
		write(1, ERR_IP_FILE, strlen(ERR_IP_FILE));
		close(fd);
		error = ERROR_CODE;
	} else if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}
	if (error != ERROR_CODE) {
		error = UTILS_readDynamic(&cadena, fd);
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

int BASIC_readClientConfig(char *name) {

	int fd = 0;
	char *cadena = NULL;
	int error;

	lseat.config.IP = NULL;
	lseat.client.nom = NULL;


	fd = FILES_openFile(name, 1);
	if (fd < 0) {
		return ERROR_CODE;
	}
	//We read the clients name through the function read Dynamic
	//Which reads dynamically controlling memory usage
	error = UTILS_readDynamic(&lseat.client.nom, fd);
	if (error < 0) {
		write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
		close(fd);
	}

	if (error > 0) {
		//Now it's time for the money
		error = UTILS_readDynamic(&cadena, fd);

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
			lseat.client.saldo = atoi(cadena);
			if (lseat.client.saldo == 0) {
				write(1, ERR_FORMAT_SALDO_FILE, strlen(ERR_FORMAT_SALDO_FILE));
				close(fd);
				error = ERROR_CODE;
			}
		}
	}

	//Now it's time for the network configuration (ip and port)
	if (error > 0) {
		error = BASIC_readNetworkConfig(fd, &lseat.config);
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
void BASIC_freeMemory() {
	if(lseat.client.nom != NULL)free(lseat.client.nom);
	if(lseat.config.IP != NULL)free(lseat.config.IP);
	if(lseat.enterprise.name != NULL)free(lseat.enterprise.name);
	if(lseat.enterprise.ip != NULL)free(lseat.enterprise.ip);
	MSTRUCTURE_destruct(&lseat.commands);
}
