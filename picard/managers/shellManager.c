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
 * This function identifies the command from the user
 * @param input commanda que s'ha introduit
 * @return
 */
int identifyCommand(char *input) {
    int i = 0;

    while (input[i]) {
        input[i] = (char) tolower(input[i]);
        i++;
    }

    if (strcmp(input, "connecta") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));
        return CMD_CONNECTA;

    } else if (strcmp(input, "mostra menu") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));
        return CMD_MENU;

    } else if (strcmp(input, "pagar") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));
        return CMD_PAGAR;

    } else if (strcmp(input, "desconnecta") == 0) {
        write(1, BYE, strlen(BYE));
        return CMD_DISCONNECT;
    } else {
        //Falta el valor de retorn. En aquesta fase no s'utilitza, ja que esta pensat per un futur.
        if(specialCommand(input) == ERROR_CODE){
            return ERROR_CODE;
        }else{
            return CMD_DEMANA;
        }

    }
}

/**
 * Manages commands with variable arguments
 * @param input
 * @return
 */
int specialCommand(char *input) {

    int error = 0, espais = 0;
    char *buffer = NULL;

    error = getArrayString(input, ' ', &espais);

    buffer = (char *) malloc(error * sizeof(char) + 1);

    if (buffer == NULL) {
        write(1, ERR_MEMORY, strlen(ERR_MEMORY));
        return ERROR_CODE;
    }

    if (error < 0) {
        write(1, COMMANDA_KO, strlen(COMMANDA_KO));
        error = ERROR_CODE;
    } else {

        strncpy(buffer, input, error);
        buffer[error] = '\0';

        if (strcmp(buffer, "elimina") == 0) {

            error = checkSpecialCommand(input + error);
            if(error != ERROR_CODE){
                error = 6;
            }

        } else if (strcmp(buffer, "demana") == 0) {

            error = checkSpecialCommand(input + error);
            if(error != ERROR_CODE){
                error = 5;
            }
        } else {

            write(1, COMMANDA_KO, strlen(COMMANDA_KO));
            error = ERROR_CODE;

        }

    }
    free(buffer);
    return error;
}

/**
 * This function checks the validity of the multi-argument command
 * @param input
 * @param index
 * @return
 */
int checkSpecialCommand(char *input) {
    int num_plats = 0;
    int total = 0, base = 0;
    char *buffer = NULL;

    total = getArrayString(input, ' ', &base);

    buffer = (char *) malloc(total * sizeof(char) + 1);

    if (buffer == NULL) {
        write(1, ERR_MEMORY, strlen(ERR_MEMORY));

        return ERROR_CODE;
    }

    if (total < 0) {

        write(1, ERR_NUM, strlen(ERR_NUM));
        total = ERROR_CODE;

    } else {

        buffer[total] = '\0';
        strncpy(buffer, input, total);

        num_plats = atoi(buffer);

        if (num_plats <= 0) {
            write(1, ERR_NUM, strlen(ERR_NUM));

            total = ERROR_CODE;

        } else {

            if (getArrayString(input + total, '\0', &base) < 0) {

                write(1, ERR_PLAT, strlen(ERR_PLAT));
                total = ERROR_CODE;

            } else {
                base = base + total;
                write(1, COMMANDA_OK, strlen(COMMANDA_OK));

            }
        }
    }
    free(buffer);
    return total;

}

int readCommands(char *cadena,int fd){
    char input[BUFFER];
    int command = 0;
    sprintf(cadena, "%s\t > ", cadena);
    write(1, cadena, strlen(cadena));
    command = setInputMode();
    if(!command){
        readInput(input, cadena, fd);
        resetInput();
        command = identifyCommand(input);
    }
    return command;
}


int loadHistory(){
    int fd;

    fd = openFile(".cmd_history", 3);

    return fd;

}