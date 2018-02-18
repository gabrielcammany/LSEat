
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

	auxiliar = (char **) realloc(history.cmdSession, (size + 2) * sizeof(char **));

	if (auxiliar != NULL) {

		history.cmdSession = auxiliar;
		history.cmdSession[size] = NULL;
		history.cmdSession[size] = (char *) malloc(sizeof(char) * strlen(input) + 2);

		if (history.cmdSession[size] != NULL) {

			memset(history.cmdSession[size], '\0', strlen(input) + 2);

			memcpy(history.cmdSession[size], input, strlen(input));
			history.lengthSession++;

		} else {

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
}

void SHELL_readInput(char **buffer) {

	int index = 0, max = 2;
	char c = ' ';

	(*buffer) = (char*) malloc(sizeof(char) * max);

	(*buffer)[0] = ' ';

	while (c != '\n') {

		read(0, &c, 1);

		if(c == '\n'){
			break;
		}

		if (c == 27) {

			read(0, &c, 1);
			read(0, &c, 1);

			continue;
		}


		if (c == 0x7f) {

			continue;
		}


		(*buffer) = (char*) realloc((*buffer),sizeof(char)* (max + 2));

		memmove(&(*buffer)[index + 1], &(*buffer)[index], sizeof(char) * (max - index + 1));

		(*buffer)[index] = c;

		SHELL_printRest((*buffer) + index, max - index - 1);

		max++;

		index++;


	}

	write(1, "\n", 1);
	(*buffer)[max - 2] = '\0';
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
			if(history.cmdHistory[i] != NULL)free(history.cmdHistory[i]);
		}
		free(history.cmdHistory);
	}
	if (history.cmdSession != NULL) {
		for (i = 0; i < history.lengthSession; i++) {
			if(history.cmdSession[i] != NULL)free(history.cmdSession[i]);
		}
		free(history.cmdSession);
	}
}
