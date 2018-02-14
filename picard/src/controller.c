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
#include "../include/interface.h"


void CONTROLLER_signalHandler(int signum) {
	switch (signum) {
		case SIGINT:

			if (socketfd > 2) CONNECTION_disconnectEnterprise(lseat.client.nom);

			free(lseat.client.nom);
			free(lseat.config.IP);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			BASIC_freeMemory();

			SHELL_resetInput();
			//INTERFACE_saveHistory();

			if (socketfd > 2)close(socketfd);

			exit(EXIT_SUCCESS);
		case SIGPIPE:
			write(1, ERR_OP, strlen(ERR_OP));

			if (socketfd > 1)close(socketfd);

			socketfd = -1;
			break;
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}


int CONTROLLER_executeCommand(Command command) {
	char *enterpriseData = NULL;

	if (command.id == CMD_CONNECTA) {
		enterpriseData = CONNECTION_data(1, lseat.config.Port, lseat.config.IP, lseat.client.nom);
		if (enterpriseData != NULL) {

			socketfd = CONNECTION_enterprise(enterpriseData, lseat.client.nom, lseat.client.saldo);
		} else {
			write(1, "Error en conectarnos a Data!\n", strlen("Error en conectarnos a Data!\n") * sizeof(char));
		}
	} else if (socketfd > 2) {

		switch (command.id) {
			case CMD_MENU:
				CONNECTION_requestMenuEnterprise();
				break;
			case CMD_DEMANA:
				CONNECTION_takeNoteEnterprise(command.data);
				break;
			case CMD_PAGAR:
				CONNECTION_payEnterprise();
				break;
			case CMD_DISCONNECT:
				CONNECTION_disconnectEnterprise(lseat.client.nom);
				return 1;
			case DEL_DISH:
				CONNECTION_deleteDishMenu(command.data);
				break;
			default:
				break;
		}
	} else {
		if(command.data != NULL){
			if (command.data[0] != NULL)free(command.data[0]);
			if (command.data[1] != NULL)free(command.data[1]);
		}
		write(1, ERR_NOCONNECTION, strlen(ERR_NOCONNECTION));
	}
	return 0;
}
