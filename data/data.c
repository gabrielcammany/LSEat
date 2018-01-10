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


int main(int argc, char *argv[]) {

	signal(SIGINT, dCONTROLLER_signalHandler);
	signal(SIGUSR1, dCONTROLLER_signalHandler);
	signal(SIGUSR2, dCONTROLLER_signalHandler);
	signal(SIGPIPE, dCONTROLLER_signalHandler);

	if (argc != 2) {
		write(1, ERR_ARGS, strlen(ERR_ARGS));
		exit(EXIT_FAILURE);
	}

	BASIC_startValues(&data);

	if (BASIC_readDataConfig(argv[1], &data) == EXIT_FAILURE) {
		write(1, ERR_FILE, strlen(ERR_FILE));
		exit(EXIT_FAILURE);
	}

	write(1, EXE, strlen(EXE));

	CONNECTION_executeData(data.enterprisePort, data.picardPort, data.ip);

	return EXIT_SUCCESS;

}