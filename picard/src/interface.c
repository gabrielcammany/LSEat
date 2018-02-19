/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */


#include "../include/interface.h"

/**
 * This function checks the validity of the multi-argument command
 * @param input data from
 * @return
 */
Command INTERFACE_checkSpecialCommand(char *input) {
	Command command;
	command.data = NULL;

	int num_plats = 0;
	int total = 0, base = 0;
	char *buffer = NULL, convert[10];

	total = UTILS_getArrayString(input, ' ', &base);

	buffer = (char*)calloc((size_t )total+1,total*sizeof(char)+1);

	if(buffer == NULL){
		write(1, ERR_MEMORY, strlen(ERR_MEMORY));

		command.id = ERROR_CODE;
		return command;
	}

	if(total < 0) {

		write(1, ERR_NUM, strlen(ERR_NUM));
		command.id = ERROR_CODE;

	} else {

		buffer[total] = '\0';
		strncpy(buffer, input, (size_t )total);

		num_plats = atoi(buffer);

		if ( num_plats <= 0 ){
			write(1, ERR_NUM, strlen(ERR_NUM));

			command.id = ERROR_CODE;

		} else {

			memset(convert,0,10);
			sprintf(convert,"%d",num_plats);

			if(UTILS_getArrayString(input+total, '\0', &base) < 0) {

				write(1, ERR_PLAT, strlen(ERR_PLAT));
				command.id = ERROR_CODE;

			} else {
				base = base+total;

				command.data = (char**) calloc (2,sizeof(char*)*2);

				if(command.data != NULL){

					command.data[0] = calloc(strlen(convert),sizeof(char)*strlen(convert));

					if(command.data[0] != NULL){

						command.data[1] = calloc(strlen(input+base),sizeof(char)*strlen(input+base));

						if(command.data[1] != NULL){

							memcpy(command.data[0],convert,strlen(convert));
							memcpy(command.data[1],input+base,strlen(input+base));

							//write(1, COMMANDA_OK, strlen(COMMANDA_OK));

						}else{

							free(command.data[0]);
							free(command.data);
							command.id = ERROR_CODE;

						}

					}else{
						free(command.data);
						command.id = ERROR_CODE;
					}

				}else{

					command.id = ERROR_CODE;

				}

			}
		}
	}

	free(buffer);

	return command;
}



/**
 * Manages commands with variable arguments
 * @param input
 * @return
 */
Command INTERFACE_specialCommand(char *input) {

	int nextObjective = 0, espais = 0;
	char *buffer = NULL;
	Command command;

	nextObjective = UTILS_getArrayString(input, ' ', &espais);

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

			command = INTERFACE_checkSpecialCommand(input + nextObjective);
			if (command.id != ERROR_CODE) {

				command.id = CMD_ELIMINA;

			}

		} else if (strcmp(buffer, "demana") == 0) {


			command = INTERFACE_checkSpecialCommand(input + nextObjective);
			if (command.id != ERROR_CODE) {

				command.id = CMD_DEMANA;

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
Command INTERFACE_identifyCommand(char *input) {

	Command command;
	command.id = ERROR_CODE;
	command.data = NULL;

	if(input != NULL){

		input = UTILS_toLower(input);

		if (strcmp(input, "connecta") == 0) {

			//write(1, COMMANDA_OK, strlen(COMMANDA_OK));
			command.id = CMD_CONNECTA;

		} else if (strcmp(input, "mostra menu") == 0) {

			//write(1, COMMANDA_OK, strlen(COMMANDA_OK));
			command.id = CMD_MENU;

		} else if (strcmp(input, "pagar") == 0) {

			//write(1, COMMANDA_OK, strlen(COMMANDA_OK));
			command.id = CMD_PAGAR;

		} else if (strcmp(input, "desconnecta") == 0) {

			command.id = CMD_DISCONNECT;

		} else {

			return INTERFACE_specialCommand(input);

		}

	}

	return command;

}


Command INTERFACE_readCommand(char *cadena) {
	char *input = NULL;
	char auxiliar[10];
	Command command;

	command.id = ERROR_CODE;
	command.data = NULL;

	sprintf(auxiliar, "%s\t > ", cadena);
	write(1, auxiliar, strlen(auxiliar));

	if (!SHELL_setInputMode()) {

		SHELL_readInput(&input);

		SHELL_resetInput();

		command = INTERFACE_identifyCommand(input);

	}
	return command;

}

void INTERFACE_saveHistory() {
	SHELL_saveToFile();
	SHELL_freeAndClose();
}

void INTERFACE_loadHistory() {
	//int fileHistory = openFile(".cmd_history", 3);
	SHELL_initializeHistory(-1);
	/*if (fileHistory != ERROR_CODE) {
		if (checkEmpty(fileHistory) > 0){
			loadBatch();
		}
	}*/
}