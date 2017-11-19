//
// Created by gabriel on 5/11/17.
//

#include "../include/dataController.h"


int readPorts(int fd, char **port){
    int error = 0;
    *port = NULL;

    //Now it's time for the picard port
    error = readDynamic(port, fd);

    //Two kinds of errors
    if (!error) {
        write(1, ERR_PORTP, strlen(ERR_PORTP));
        close(fd);
        error = ERROR_CODE;
    } else if (error < 0) {
        write(1, ERR_MEMORY, strlen(ERR_MEMORY));
        close(fd);
        error = ERROR_CODE;
    }

    return error;
}

int readDataConfig(char* name,Data *data) {

    int fd = 0;
    int error;

    data->config.IP = NULL;

    fd = openFile(name, 1);
    if (fd < 0) {
        return ERROR_CODE;
    }
    //We read Data IP from config file
    error = readDynamic(&data->config.IP, fd);
    if (error < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
        close(fd);
        error = ERROR_CODE;
    }

    if (error > 0) {
        error = readPorts(fd,&data->config.picardPort);
        if ( error > 0 ) {
            error = readPorts(fd,&data->config.enterprisePort);

        }
    }

    close(fd);

    return error;
}