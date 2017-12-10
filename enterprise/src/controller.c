/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/controller.h"

void eCONTROLLER_signalHandler(int signum) {
    switch (signum) {
        case SIGINT:

			if(socketData > 2)close(socketData);
			if(socketPic > 2)close(socketPic);

			BASIC_freeMemory();

			pthread_kill(enterprise.thread_data,SIGKILL);
			pthread_join(enterprise.thread_data, NULL);

            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);

		case SIGUSR1:

			if(socketData > 2)close(socketData);

			pthread_kill(enterprise.thread_data,SIGKILL);
			pthread_join(enterprise.thread_data, NULL);

			exit(EXIT_FAILURE);

        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}