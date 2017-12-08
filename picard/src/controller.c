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


void CONTROLLER_signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			free(lseat.client.nom);
			free(lseat.config.IP);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			SHELL_resetInput();
			INTERFACE_saveHistory();

			close(socketfd);

			exit(EXIT_SUCCESS);
		case SIGPIPE:
			write(1, ERR_OP, strlen(ERR_OP));
			write(1, ERR_ENTDISC, strlen(ERR_ENTDISC));
			close(socketfd);
			socketfd = -1;
			break;
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}


int CONTROLLER_executeCommand(Command command, ClientLSEat lseat) {
	char *enterpriseData = NULL;

	if(command.id == CMD_CONNECTA) {
		enterpriseData = connection_data(lseat.config.Port, lseat.config.IP, lseat.client.nom);
		if (enterpriseData != NULL) {
			socketfd = connection_enterprise(enterpriseData, lseat.client.nom, lseat.client.saldo);
		}
	}else if(socketfd > 0){
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
	}else{
		write(1,ERR_NOCONNECTION,strlen(ERR_NOCONNECTION));
	}
	return 0;
}
