
#include "../include/shell.h"

History history;

struct termios saved_attributes;

void SHELL_resetInput() {
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

int SHELL_setInputMode(void) {
	struct termios tattr;

	/* Asegurarnos del terminal */
	if (!isatty(STDIN_FILENO)) {
		//No es un terminal!
		return ERROR_CODE;
	}

	/* Desarem els atributs per poder restaurar el terminal en el seu estat inicial*/
	tcgetattr(STDIN_FILENO, &saved_attributes);

	/* Canvi dels atributs. */
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON | ECHO | ECHOE);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
	return 0;
}

int SHELL_loadNextCommand() {

	char **auxiliar = NULL, buffer[BUFFER], *aux = NULL;
	int size = history.length;

	memset(&buffer, 0, BUFFER);

	if (read(history.historyfd, &buffer, BUFFER) > 0) {
		printf("Commanda -%s-\n", buffer);

		auxiliar = (char **) realloc(history.cmdHistory, size * sizeof(char **) + 1);

		if (auxiliar == NULL) {

			return -1;

		} else {

			history.cmdHistory = auxiliar;
			history.cmdHistory[size] = NULL;
			aux = (char *) realloc(history.cmdHistory[size], sizeof(char) * strlen(buffer));

			if (aux == NULL) {

				auxiliar = (char **) realloc(history.cmdHistory, size * sizeof(char *));

				if (auxiliar == NULL) {

					int i;
					for (i = 0; i < size; i++) {

						free(history.cmdHistory[i]);

					}

					free(history.cmdHistory);
				}

				return -1;

			} else {

				history.cmdHistory[size] = aux;
				memset(history.cmdHistory[size], 0, BUFFER);
				strcpy(history.cmdHistory[size], buffer);
				printf("Commanda -%s-\n", buffer);

			}

			history.length++;
		}

	}

	return 1;
}

int SHELL_loadBatch() {
	int i;
	for (i = 0; i < BATCH_HISTORY / 2 && SHELL_loadNextCommand() > 0; i++) {}
	history.length += i;
	return i == 0 ? -1 : i;
}

void SHELL_saveCommand(char *input) {

	char **auxiliar = NULL;
	int size = history.lengthSession;

	auxiliar = (char **) realloc(history.cmdSession, (size + 1) * sizeof(char **));

	if (auxiliar != NULL) {

		history.cmdSession = auxiliar;
		history.cmdSession[size] = NULL;
		history.cmdSession[size] = (char *) malloc((strlen(input) + 1) * sizeof(char));

		if (history.cmdSession[size] != NULL) {

			memset(history.cmdSession[size], 0, strlen(input));

			strcpy(history.cmdSession[size], input);

			history.lengthSession++;

		} else {

			if (size > 1) {

				auxiliar = (char **) realloc(history.cmdSession, size * sizeof(char **));

				if (auxiliar == NULL) {

					int i;

					for (i = 0; i < size; i++) {

						free(history.cmdSession[i]);

					}

					free(history.cmdSession);

				} else {

					history.cmdSession = auxiliar;

				}
			} else {
				free(history.cmdSession);
			}

		}
	}
}

void SHELL_saveToFile() {
	int i = 0;
	char buffer[BUFFER];

	memset(buffer, 0, BUFFER);

	for (i = 0; i < history.lengthSession; i++) {

		memset(buffer, 0, BUFFER);
		strcpy(buffer, history.cmdSession[i]);

		if (write(history.historyfd, buffer, BUFFER) < 0) {

			break;

		}
	}

	if (i < BATCH_HISTORY) {

		for (i = 0; i < history.length && i + history.lengthSession < BATCH_HISTORY; i++) {

			memset(buffer, 0, BUFFER);
			strcpy(buffer, history.cmdHistory[i]);

			if (write(history.historyfd, buffer, BUFFER) < 0) {

				break;

			}

		}

	}
}

void SHELL_printRest(char* buffer, int max){
	int i;
	for(i = 0; i < max; i++){
		write(1, &buffer[i], sizeof(char));
	}
	write(1,'\0', sizeof(char));
}

char* SHELL_readInput(char *buffer, char *menu) {

	int index = 0, max = 2, hEnabled = 1,
			command = history.length, commandSession = history.lengthSession;
	char c = ' ', aux[10];

	buffer = (char*) malloc(sizeof(char));

	while (c != '\n') {

		read(0, &c, 1);

		if (c == '\n') {
			break;
		}

		if (c == 27) {
			read(0, &c, 1);
			read(0, &c, 1);
			switch (c) {
				case 'A': //Move up the history
					if (history.lengthSession > 0 || history.length > 0) {

						if (commandSession > 0) {

							commandSession--;
							printf("Buffer load size: %d\n",(int)strlen(history.cmdSession[commandSession]) + 2);
							strcpy(buffer, history.cmdSession[commandSession]);

						} else if (command > 0) {

							command--;
							printf("Buffer load size: %d\n",(int)strlen(history.cmdHistory[command]) + 2);
							strcpy(buffer, history.cmdHistory[command]);

						} else {

							hEnabled = 0;

							continue;

						}
						if (hEnabled > 0) {

							hEnabled = 1;
							max = (int) strlen(buffer);
							index = max - 1;

							write(1, NETEJAR_LINIA, strlen(NETEJAR_LINIA));
							write(1, "\r", 1);
							write(1, menu, strlen(menu));

							SHELL_printRest(buffer,max - 1);

							sprintf(aux, "\033[%dD", 1);
							write(1, aux, strlen(aux));

						}
					}
					break;
				case 'D': //Move left through command
					if (index > 0) {

						write(1, ESQUERRA, strlen(ESQUERRA));
						index--;

					}
					break;
				case 'C': //Move rigth through the command
					if (index < max - 1) {

						write(1, DRETA, strlen(DRETA));
						index++;

					}
					break;
				case 'B': //Move down history
					if (history.lengthSession > 0 || history.length > 0) {

						if (commandSession < history.lengthSession) {

							printf("Buffer load size: %d\n",(int)strlen(history.cmdSession[commandSession]));

							buffer = realloc(buffer, sizeof(char) * strlen(history.cmdSession[commandSession]) + 1);

							strcpy(buffer, history.cmdSession[commandSession]);


							commandSession++;


						} else if (command < history.length) {

							printf("Buffer load size: %d\n",(int)strlen(history.cmdHistory[command]));
							buffer = realloc(buffer, sizeof(char) * strlen(history.cmdHistory[command]) + 1);

							strcpy(buffer, history.cmdHistory[command]);

							command++;


						} else {

							hEnabled = 2;

							continue;

						}
						if (hEnabled < 2) {

							max = (int) strlen(buffer);
							index = max - 1;

							write(1, NETEJAR_LINIA, strlen(NETEJAR_LINIA));
							write(1, "\r", 1);
							write(1, menu, strlen(menu));

							SHELL_printRest(buffer,max - 1);

							sprintf(aux, "\033[%dD", 2);
							write(1, aux, strlen(aux));


						}
					}

					break;
				default:
					break;
			}

			continue;
		}

		//ASCII number in hexa for delete
		if (c == 0x7f) {

			if (index > 0) {

				memmove(&buffer[index - 1], &(buffer[index]), sizeof(char));

				buffer[index] = ' ';

				sprintf(aux, "\033[%dD", 1);
				write(1, aux, strlen(aux));

				SHELL_printRest(buffer+(index),(max) - index);

				sprintf(aux, "\033[%dD", 2);
				write(1, aux, strlen(aux));

				index--;

				buffer = realloc(buffer, sizeof(char) * max + 1);

				if (max > 0 && max >= index)max--;


			}

			continue;
		}

		if (index < BUFFER) {

			memmove(&buffer[index + 1], &buffer[index], sizeof(char));

			buffer[index] = c;

			SHELL_printRest(buffer+(index),(max - index - 1));

			max++;

			buffer = realloc(buffer, sizeof(char) * max + 2);

			index++;
		}


	}

	buffer[max - 1] = '\0';
	write(1, "\n", 1);
	if (!UTILS_checkEmptyString(buffer))SHELL_saveCommand(buffer);

	buffer[max - 2] = '\0';
	printf("buffer -%s-\n",buffer);
	return buffer;
}



void SHELL_initializeHistory(int fd) {
	history.length = 0;
	history.lengthSession = 0;
	history.cmdSession = NULL;
	history.cmdHistory = NULL;
	history.historyfd = fd;
}


void SHELL_freeAndClose() {
	int i = 0;
	if (history.cmdHistory != NULL) {
		for (i = 0; i < history.length; i++) {
			free(history.cmdHistory[i]);
		}
		free(history.cmdHistory);
	}
	if (history.cmdSession != NULL) {
		for (i = 0; i < history.lengthSession; i++) {
			free(history.cmdSession[i]);
		}
		free(history.cmdSession);
	}
	if(history.historyfd > 1)close(history.historyfd);
}
