//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"

void control_signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			free(Data.picardPort);
			free(Data.enterprisePort);
			free(Data.IP);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			close(socketfd);

			exit(EXIT_SUCCESS);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}

int control_executeData(Data *data){

	basic_freeMemory(data);
	return EXIT_SUCCESS;
}