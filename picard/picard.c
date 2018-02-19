/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "include/controller.h"


int main(int argc, char **argv) {
	Command command;

	int error = 0;

	signal(SIGINT, CONTROLLER_signalHandler);
	signal(SIGPIPE, CONTROLLER_signalHandler);

	if (argc != 2) {
		write(1, ERR_ARG, strlen(ERR_ARG));
		exit(EXIT_FAILURE);
	}

	//we initialize all variables
	BASIC_startValues(&command);

	//Read configuration for the picard
	if (BASIC_readClientConfig(argv[1]) < 0) {
		exit(EXIT_FAILURE);
	}

	//start message when executing picard client
	BASIC_startupMessages();

	while (!error) {

		command = INTERFACE_readCommand(lseat.client.nom);
		//after checking the command id and reading its data
		//from user, then we execute the command it represents
		if (command.id != ERROR_CODE)error = CONTROLLER_executeCommand(command);

	}

	write(1, BYE, strlen(BYE));

	BASIC_freeMemory();

	close(socketfd);

	return EXIT_SUCCESS;
}
