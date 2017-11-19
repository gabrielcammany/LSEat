//
// Created by gabriel on 5/11/17.
//

#ifndef LSEAT_DATAMANAGER_H
#define LSEAT_DATAMANAGER_H

//Includes propios
#include "../../lib/includes/utils.h"
#include "../../lib/includes/files.h"

//Constantes propias
#define ERR_PORTP "Error en llegir el port del Picard!\n"
#define ERR_PORTE "Error en llegir el port del Enterprise!\n"
#define ERR_IP "Error en llegir la IP de Data\n"
#define ERR_MEMORY "Hi ha un problema amb la memoria del sistema.\n"
#define ERR_FORMAT_PORT "Hi ha un problema amb el format del port!\n"

//Tipos propios
/**
 * Structure to save data configuration
 * IP: IP of the server data
 * picardPort: Port where picards will connect
 * enterprisePort: Port where enterprise will connect
 */
typedef struct{
    char* IP;
    char* picardPort;
    char* enterprisePort;
}DataConf;

/**
 * Structure to save information of Picards
 * IP: IP from picard
 * port: Port to connect to picard
 */
typedef struct{
    char* IP;
    int port;
}Picard;

/**
 * Structure to save information of Enterprise
 * IP: IP from enterprise
 * port: Port to connect to enterprise
 */
typedef struct{
    char* IP;
    int port;
}Enterprise;

/**
 * Global structure that forms data
 */
typedef struct {
    DataConf config;
    Picard picard;
    Enterprise enterprise;
}Data;

/**
 * Function to get ports from files
 * @param fd file descriptor from  where to read
 * @param port port readed
 * @return
 */
int readPorts(int fd, char **port);

/**
 * Function which reads the configuration of data
 * @param data Global structure of Data
 * @return if there is an error or not
 */
int readDataConfig(char* name,Data *data);



#endif //LSEAT_DATAMANAGER_H
