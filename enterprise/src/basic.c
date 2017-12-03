//
// Created by gabriel on 19/11/17.
//

#include "../include/basic.h"
#include "../../lib/include/network.h"

void basic_freeMemory() {


}

/*void basic_startValues(Enterprise *enterprise) {

}*/

int basic_readPorts(int fd, int *portNumber) {
    int error = 0;
    char *port = NULL;

    //Now it's time for the picard port
    error = readDynamic(&port, fd);

    //Two kinds of errors
    if (error <= 0) {
        return EXIT_FAILURE;
    }

    *portNumber = atoi(port);
    return EXIT_SUCCESS;

}

int basic_readConfigEnterprise(char *fitxer, char *menu, Enterprise *enterprise) {
    int fd = 0, i = 0;
    int error = 0;

    fd = openFile(fitxer, 1);
    if (fd == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    //We read the restaurant name
    if (readDynamic(&enterprise->restaurant.name, fd) < 0) {
        write(1, ERR_ENTNAME, strlen(ERR_ENTNAME));
        close(fd);
        return EXIT_FAILURE;
    }

    //we read restaurant seconds to refresh data info
    if ((error = basic_readPorts(fd, &enterprise->restaurant.seconds)) == EXIT_FAILURE) {
        write(1, ERR_SEC, strlen(ERR_SEC));
        close(fd);
        return EXIT_FAILURE;
    }

    //we read IP to connect to data
    if (readDynamic(&enterprise->data.ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
        close(fd);
        return EXIT_FAILURE;
    }

    //We read the datas port
    if ((error = basic_readPorts(fd, &enterprise->data.port)) == EXIT_FAILURE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
        close(fd);
        return EXIT_FAILURE;
    }

    //same as above but from where picards will connect
    if (readDynamic(&enterprise->picard.ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
        close(fd);
        return EXIT_FAILURE;
    }

    if ((error = basic_readPorts(fd, &enterprise->picard.port)) == EXIT_FAILURE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);

    //Show welcome message
    write (1, WELCOME, strlen(WELCOME));
    write (1, enterprise->restaurant.name, strlen(enterprise->restaurant.name));
    write (1, "\n\0", sizeof(char)*2);

    //Now its time for the menu structure
    fd = openFile(menu, 1);
    enterprise->menu = (Dish*) malloc (sizeof(Dish));

    //because this functions returns the number of bytes we read
    //we can know when its end of file
    while( readDynamic(&enterprise->menu[i].name, fd) > 0 ) {
        basic_readPorts(fd, &enterprise->menu[i].unities);
        basic_readPorts(fd, &enterprise->menu[i].price);
        i++;
        enterprise->menu = (Dish*) realloc (enterprise->menu, sizeof(Dish)*(i+1));
    }

    write(1, MENU_READY, strlen(MENU_READY));
    enterprise->num_menu = i;
    close(fd);

    return error;
}

