//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"

void dCONTROLLER_signalHandler(int signum) {

	switch (signum) {
		case SIGINT:

			BASIC_freeMemory(&data);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			close(socketEnt);
			close(socketPic);

			exit(EXIT_SUCCESS);

		case SIGUSR1:

			BASIC_freeMemory(&data);

			write(1, "\n", strlen("\n"));
			write(1, NBYE, strlen(NBYE));

			close(socketEnt);
			close(socketPic);

			exit(EXIT_FAILURE);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}
