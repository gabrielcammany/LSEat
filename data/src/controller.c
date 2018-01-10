//
// Created by gabriel on 5/11/17.
//

#define MIN_FD 2

#include "../include/controller.h"

void dCONTROLLER_signalHandler(int signum) {

	switch (signum) {
		case SIGINT:

			BASIC_freeMemory(&data);
			HASH_destruct(&enterprise);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			if(socketEnt > 0 && NETWORK_openedSocket(socketEnt) > 0)close(socketEnt);
			if(socketPic > 0 && NETWORK_openedSocket(socketPic) > 0)close(socketPic);

			pthread_kill(thread_id, SIGUSR2);
			pthread_join(thread_id, NULL);

			exit(EXIT_SUCCESS);

		case SIGUSR1:

			BASIC_freeMemory(&data);
			HASH_destruct(&enterprise);

			write(1, "\n", strlen("\n"));
			write(1, NBYE, strlen(NBYE));

			if(socketEnt > 0 && NETWORK_openedSocket(socketEnt) > 0)close(socketEnt);
			if(socketPic > 0 && NETWORK_openedSocket(socketPic) > 0)close(socketPic);

			exit(EXIT_FAILURE);

		case SIGUSR2:
			pthread_exit(0);
		case SIGPIPE:
			printf("Error pipe!\n");
			break;

		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}
