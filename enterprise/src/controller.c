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
	Packet packet;

    switch (signum) {
        case SIGINT:

			if(NETWORK_openedSocket(socketData) > 0 && socketData > 0){

				close(socketData);

				if ((socketData = NETWORK_createConnectionClient(atoi(enterprise.config.data_port), enterprise.config.data_ip)) > 0) {

					packet = NETWORK_createPacket(DISCONNECT, HEADER_DATPIC, (int)strlen(enterprise.config.picard_port), enterprise.config.picard_port);

					if(packet.type != ERROR_CODE){

						NETWORK_sendSerialized(socketData, packet);
						NETWORK_freePacket(&packet);
					}
					close(socketData);
				}

			}
			if(NETWORK_openedSocket(socketPic) > 0 && socketPic > 0)close(socketPic);

			eBASIC_freeMemory();

			pthread_kill(enterprise.thread_data, SIGUSR2);
			pthread_join(enterprise.thread_data, NULL);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);

		case SIGUSR1:

			pthread_kill(enterprise.thread_data, SIGUSR2);
			pthread_join(enterprise.thread_data, NULL);

			eBASIC_freeMemory();

			if(socketData > 2)close(socketData);
			if(socketPic > 2)close(socketPic);


			exit(EXIT_FAILURE);

		case SIGUSR2:
			pthread_exit(0);

        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}