/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */
#include "shellManager.h"

/**
 * This function checks the validity of the multi-argument command
 * @param input
 * @param index
 * @return
 */
Command checkSpecialCommand(char *input) {
	int total = 0, base = 0,num_plats = 0;
	char *buffer = NULL;
	Command command;

	total = getArrayString(input, ' ', &base);

	buffer = (char *) malloc(total * sizeof(char) + 1);
	memset(buffer,0,total * sizeof(char) + 1);

	if (buffer == NULL) {
		write(1, ERR_MEMORY, strlen(ERR_MEMORY));
		command.id = ERROR_CODE;
		return command;
	}

	if (total < 0) {

		write(1, ERR_NUM, strlen(ERR_NUM));
		command.id = ERROR_CODE;
		free(buffer);
		return command;

	} else {

		buffer[total] = '\0';
		strncpy(buffer, input, total);

		num_plats = atoi(buffer);

		if (num_plats < 1) {

			write(1, ERR_NUM, strlen(ERR_NUM));

			command.id = ERROR_CODE;
			free(buffer);

			return command;

		} else {

			command.data = (char **)malloc(sizeof(char *) * 2);

			if(command.data == NULL){

				command.id = ERROR_CODE;
				write(1, ERR_MEMORY, strlen(ERR_MEMORY));
				free(buffer);
				return command;

			}

			command.data[0] = (char *)malloc(total * sizeof(char));

			if(command.data[0] == NULL){

				command.id = ERROR_CODE;
				write(1, ERR_MEMORY, strlen(ERR_MEMORY));
				free(buffer);
				return command;

			}

			strcpy(command.data[0],buffer);


			if (getArrayString(input + total, '\0', &base) < 0) {

				write(1, ERR_PLAT, strlen(ERR_PLAT));

				command.id = ERROR_CODE;

			} else {

				base = base + total;

				command.data[1] = (char *)malloc(strlen(input+base) * sizeof(char));

				if(command.data[1] == NULL){

					command.id = ERROR_CODE;
					write(1, ERR_MEMORY, strlen(ERR_MEMORY));
					free(buffer);
					free(command.data[0]);
					free(command.data);

					return command;

				}

				write(1, COMMANDA_OK, strlen(COMMANDA_OK));

			}
		}
	}
	return command;

}


/**
 * Manages commands with variable arguments
 * @param input
 * @return
 */
Command specialCommand(char *input) {

	int nextObjective = 0, espais = 0;
	char *buffer = NULL;
	Command command;

	nextObjective = getArrayString(input, ' ', &espais);

	buffer = (char *) malloc(nextObjective * sizeof(char) + 1);

	if (buffer == NULL) {

		write(1, ERR_MEMORY, strlen(ERR_MEMORY));
		command.id = ERROR_CODE;
		return command;

	}

	if (nextObjective < 0) {

		write(1, COMMANDA_KO, strlen(COMMANDA_KO));
		command.id = ERROR_CODE;
		command.data = NULL;

	} else {

		strncpy(buffer, input, nextObjective);
		buffer[nextObjective] = '\0';
		command.id = ERROR_CODE;
		command.data = NULL;

		if (strcmp(buffer, "elimina") == 0) {

			command = checkSpecialCommand(input + nextObjective);
			if (command.id != ERROR_CODE) {

				command.id = 6;

			}

		} else if (strcmp(buffer, "demana") == 0) {

			command = checkSpecialCommand(input + nextObjective);
			if (command.id != ERROR_CODE) {

				command.id = 5;

			}
		} else {

			write(1, COMMANDA_KO, strlen(COMMANDA_KO));

		}

	}

	free(buffer);
	return command;
}

/**
 * This function identifies the command from the user
 * @param input commanda que s'ha introduit
 * @return
 */
Command identifyCommand(char *input) {

	Command command;

	input = toLower(input);

	if (strcmp(input, "connecta") == 0) {

		write(1, COMMANDA_OK, strlen(COMMANDA_OK));
		command.id = CMD_CONNECTA;
		command.data = NULL;

		return command;

	} else if (strcmp(input, "mostra menu") == 0) {

		write(1, COMMANDA_OK, strlen(COMMANDA_OK));
		command.id = CMD_MENU;
		command.data = NULL;
		return command;

	} else if (strcmp(input, "pagar") == 0) {

		write(1, COMMANDA_OK, strlen(COMMANDA_OK));
		command.id = CMD_PAGAR;
		command.data = NULL;
		return command;

	} else if (strcmp(input, "desconnecta") == 0) {

		write(1, BYE, strlen(BYE));
		command.id = CMD_DISCONNECT;
		command.data = NULL;
		return command;

	} else {

		return specialCommand(input);

	}
}


Command readCommands(char *cadena) {
	char input[BUFFER];
	char auxiliar[10];
	Command command;
	command.id = ERROR_CODE;

	sprintf(auxiliar, "%s\t > ", cadena);
	write(1, auxiliar, strlen(auxiliar));

	if (!setInputMode()) {

		readInput(input, auxiliar);

		resetInput();

		command = identifyCommand(input);

	}

	return command;
}

void saveHistory() {
	//saveToFile();
	freeAndClose();
}

void loadHistory() {
	//int fileHistory = openFile(".cmd_history", 3);
	initializeHistory(-1);
	/*if (fileHistory != ERROR_CODE) {
		if (checkEmpty(fileHistory) > 0){
			loadBatch();
		}
	}*/
}