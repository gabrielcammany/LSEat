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


int executeCommand(char *input) {
    int i = 0;

    while (input[i]) {
        input[i] = (char) tolower(input[i]);
        i++;
    }

    if (strcmp(input, "connecta") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    } else if (strcmp(input, "mostra menu") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    } else if (strcmp(input, "pagar") == 0) {
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    } else if (strcmp(input, "desconnecta") == 0) {
        write(1, BYE, strlen(BYE));
        return 1;
    } else {
        //Falta el valor de retorn. En aquesta fase no s'utilitza, ja que esta pensat per un futur.
        specialCommand(input);
    }
    return 0;
}

int specialCommand(char *input) {

    int error = 0, espais = 0;
    char *buffer = NULL;

    error = getArrayString(input, ' ', &espais);

    buffer = (char *) malloc(error * sizeof(char) + 1);

    if (buffer == NULL) {
        write(1, ERR_MEMORY, strlen(ERR_MEMORY));
        return 1;
    }

    if (error < 0) {
        write(1, COMMANDA_KO, strlen(COMMANDA_KO));
        error = 1;
    } else {

        strncpy(buffer, input, error);
        buffer[error] = '\0';

        if (strcmp(buffer, "elimina") == 0) {

            error = checkSpecialCommand(input + error);

        } else if (strcmp(buffer, "demana") == 0) {

            error = checkSpecialCommand(input + error);

        } else {

            write(1, COMMANDA_KO, strlen(COMMANDA_KO));
            error = 1;

        }

    }
    free(buffer);
    return error;
}

int checkSpecialCommand(char *input) {
    int num_plats = 0;
    int total = 0, base = 0;
    char *buffer = NULL;

    total = getArrayString(input, ' ', &base);

    buffer = (char *) malloc(total * sizeof(char) + 1);

    if (buffer == NULL) {
        write(1, ERR_MEMORY, strlen(ERR_MEMORY));

        return 1;
    }

    if (total < 0) {

        write(1, ERR_NUM, strlen(ERR_NUM));
        total = 1;

    } else {

        buffer[total] = '\0';
        strncpy(buffer, input, total);

        num_plats = atoi(buffer);

        if (num_plats <= 0) {
            write(1, ERR_NUM, strlen(ERR_NUM));

            total = 1;

        } else {

            if (getArrayString(input + total, '\0', &base) < 0) {

                write(1, ERR_PLAT, strlen(ERR_PLAT));
                total = 1;

            } else {
                base = base + total;
                write(1, COMMANDA_OK, strlen(COMMANDA_OK));

            }
        }
    }
    free(buffer);
    return total;

}
