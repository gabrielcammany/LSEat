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
	signal(SIGINT, control_signalHandler);

	if (argc != 2) {
		write(1, ERR_ARG, strlen(ERR_ARG));
		exit(EXIT_FAILURE);
	}

	//we initialize all variables
	basic_startValues(&command);

	//Read configuration for the picard
    error = basic_readClientConfig(argv[1], &lseat);

    if(error < 0){
        exit(EXIT_FAILURE);
    }
	//start message when executing picard client
	basic_startupMissages();

	interface_loadHistory();

	while (error == 0) {

		command = interface_readCommand(lseat.client.nom);
		//after checking the command id and reading its data
		//from user, then we execute the command it represents
		error = control_executeCommand(command, lseat);
	}

	write(1, BYE, strlen(BYE));

	interface_saveHistory();

	basic_freeMemory();

	close(socketfd);

	return EXIT_SUCCESS;
}
