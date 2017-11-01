/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

//includes del sistema
#include <signal.h>

//Includes propios
#include "managers/shellManager.h"
#include "managers/connectionManager.h"

//Constantes
#define ERR_ARG "Error en el nombre d'arguments!\n"
#define WELCOME "BENVINGUT %s\n"
#define SALDO "Té %d euros disponibles\n"
#define ERR_INT "Interrupció desconeguda!\n"

ClientLSEat lseat;
int socketfd;

/**
 * Funcion encargada de borrar la memoria dinamica
 * @param lseat Variable a borrar
 */
void freeMemory() {

	free(lseat.client.nom);
	free(lseat.config.IP);

}


void signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			free(lseat.client.nom);
			free(lseat.config.IP);
			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));
			resetInput();
			close(socketfd);
			exit(0);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}


int main(int argc, char **argv) {
	char cadena[100];
	int command = 0, fd = 0;

	signal(SIGINT, signalHandler);

	if (argc != 2) {
		write(1, ERR_ARG, strlen(ERR_ARG));
		exit(EXIT_FAILURE);
	}

	socketfd = connectToEnterprise(&lseat, argv[1]);

	//Missatges de entrada del usuari
	sprintf(cadena, WELCOME, lseat.client.nom);
	write(1, cadena, strlen(cadena));

	sprintf(cadena, SALDO, lseat.client.saldo);
	write(1, cadena, strlen(cadena));

	write(1, INTRODUCTION, strlen(INTRODUCTION));

	fd = loadHistory();

	while ( command > 0) {

		command = readCommands(lseat.client.nom,fd);

	}

	//Tornem a definir el input anterior

	freeMemory();

	close(socketfd);

	return 0;
}
