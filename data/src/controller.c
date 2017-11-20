//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"

void dControl_signalHandler(int signum) {

	switch (signum) {
		case SIGINT:

			basic_freeMemory(&data);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			close(socketfd);

			exit(EXIT_SUCCESS);

		case SIGUSR1:

			basic_freeMemory(&data);

			write(1, "\n", strlen("\n"));
			write(1, NBYE, strlen(BYE));

			close(socketfd);

			exit(EXIT_FAILURE);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}
