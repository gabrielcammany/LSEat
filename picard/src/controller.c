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


void control_signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			free(lseat.client.nom);
			free(lseat.config.IP);

			write(1, "\n", strlen("\n"));
			write(1, BYE, strlen(BYE));

			resetInput();
			interface_saveHistory();

			close(socketfd);

			exit(EXIT_SUCCESS);
		default:
			write(1, ERR_INT, strlen(ERR_INT));
			break;
	}

}




int control_executeCommand(Command command, ClientLSEat lseat) {
	char * enterpriseData = NULL;
	switch (command.id) {
		case CMD_CONNECTA:
			enterpriseData = connection_data(lseat.config.Port, lseat.config.IP, lseat.client.nom);
			if (enterpriseData != NULL) {
				socketfd = connection_enterprise(enterpriseData,lseat.client.nom,lseat.client.saldo);
			}
			break;
		case CMD_MENU:
			connection_requestMenuEnterprise();
			break;
		case CMD_DEMANA:
 			connection_takeNoteEnterprise(command.data);
			break;
		case CMD_PAGAR:
			connection_payEnterprise();
			break;
		case CMD_DISCONNECT:
			connection_disconnectEnterprise(lseat.client.nom);
			return 1;
			break;
		case DEL_DISH:
			connection_deleteDishMenu(command.data);
			break;
		default:
			break;
	}
	return 0;
}
