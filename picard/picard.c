#include <signal.h>
#include "include/picardController.h"

/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */


int main(int argc, char **argv) {
	Command command;

	signal(SIGINT, signalHandler);

	if (argc != 2) {
		write(1, ERR_ARG, strlen(ERR_ARG));
		exit(EXIT_FAILURE);
	}

	startValues(&command);

	socketfd = connectToEnterprise(&lseat, argv[1]);

	startupMissages();

	loadHistory();

	while (command.id != 0) {

		command = readCommands(lseat.client.nom);

	}

	write(1, BYE, strlen(BYE));

	saveHistory();

	freeMemory();

	close(socketfd);

	return EXIT_SUCCESS;
}
