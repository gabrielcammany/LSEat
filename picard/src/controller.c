//
// Created by gabriel on 19/11/17.
//

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




void control_executeCommand(Command command, ClientLSEat lseat) {
	char * enterpriseData = NULL;

	switch (command.id) {
		case CMD_CONNECTA:
			enterpriseData = connection_data(lseat.config.Port, lseat.config.IP, lseat.client.nom);
			/*if (enterpriseData != NULL) {
				socketfd = connection_enterprise(enterpriseData,lseat.client.nom,lseat.client.saldo);
			}*/
			break;
		case CMD_MENU:
			break;
		case CMD_DEMANA:
			break;
		case CMD_PAGAR:
			break;
		case CMD_DISCONNECT:
			break;
		default:
			break;
	}
}
