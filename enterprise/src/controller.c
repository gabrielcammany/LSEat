//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"


void dControl_signalHandler(int signum) {

    switch (signum) {
        case SIGINT:

            basic_freeMemory(&enterprise);

            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);
            break;

        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}


int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise) {
    return basic_readConfigEnterprise(fitxer,menu, enterprise);
}