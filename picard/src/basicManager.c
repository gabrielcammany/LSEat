//
// Created by gabriel on 5/11/17.
//

#include <termio.h>
#include <unistd.h>
#include "signal.h"
#include <memory.h>
#include "../../lib/includes/files.h"
#include "../include/picardController.h"

void startupMissages() {
	char cadena[100];
	//Missatges de entrada del usuari
	sprintf(cadena, WELCOME, lseat.client.nom);
	write(1, cadena, strlen(cadena));

	sprintf(cadena, SALDO, lseat.client.saldo);
	write(1, cadena, strlen(cadena));

	write(1, INTRODUCTION, strlen(INTRODUCTION));
}

void startValues(Command *command){
	lseat.client.nom = NULL;
	lseat.client.saldo = 0;
	lseat.config.IP = NULL;
	lseat.config.Port = 0;
	command->data = NULL;
	command->id = -1;
}

void signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			free(lseat.client.nom);
			free(lseat.config.IP);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			resetInput();
			saveHistory();

			close(socketfd);

			exit(EXIT_SUCCESS);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}

int readClientConfig(char *name, ClientLSEat *lseat) {

	int fd = 0;
	char *cadena = NULL;
	int error;

	lseat->config.IP = NULL;
	lseat->client.nom = NULL;


	fd = openFile(name, 1);
	if (fd < 0) {
		return ERROR_CODE;
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
			error = ERROR_CODE;
		} else if (error < 0) {
			write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
			close(fd);
		} else {
			lseat->client.saldo = atoi(cadena);
			if (lseat->client.saldo == 0) {
				write(1, ERR_FORMAT_SALDO_FILE, strlen(ERR_FORMAT_SALDO_FILE));
				close(fd);
				error = ERROR_CODE;
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


int readNetworkConfig(int fd, Config *config) {
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

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void freeMemory() {

	free(lseat.client.nom);
	free(lseat.config.IP);

}
