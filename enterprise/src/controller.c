//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"
#include "../include/connection.h"

void dControl_signalHandler(int signum) {

    switch (signum) {
        case SIGINT:
            close(socketData);
            //close(socketPic);

            //basic_freeMemory(&enterprise);

            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);
            break;
        case SIGALRM:
            alarm(enterprise.restaurant.seconds);
            connection_handlerData();
            break;
        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}


int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise) {
    return basic_readConfigEnterprise(fitxer,menu, enterprise);
}