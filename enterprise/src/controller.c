//
// Created by gabriel on 5/11/17.
//

#include "../include/controller.h"
#include "../include/connection.h"

void dControl_signalHandler(int signum) {
    char convert[10];

    switch (signum) {
        case SIGINT:
            close(socketData);
            close(socketPic);

            //basic_freeMemory(&enterprise);

            write(1, "\n", strlen("\n"));
            write(1, BYE, strlen(BYE));

            exit(EXIT_SUCCESS);
            break;
        case SIGALRM:
            printf("SEGONS: %d\n",enterprise.restaurant.seconds);
            write(1,"Han pasat ",strlen("Han pasat "));
            sprintf(convert,"%d",enterprise.restaurant.seconds);
            write(1,convert, sizeof(char));
            write(1," segons\n", strlen(" segons\n"));
            alarm(enterprise.restaurant.seconds);
            sendInfoData();
            break;
        default:
            write(1, ERR_INT, strlen(ERR_INT));
            break;
    }

}


int controller_readConfiguration(char *fitxer, char *menu, Enterprise *enterprise) {
    return basic_readConfigEnterprise(fitxer,menu, enterprise);
}