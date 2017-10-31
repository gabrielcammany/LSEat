/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 26-10-2017
 */


#include "fitxers.h"


LSEat lecturaFitxerConfigClient(char nombreFichero[]) {

    int fd = 0;
    char *cadena = NULL;
    LSEat lseat;
    int error;

    lseat.config.IP = NULL;
    lseat.client.nom = NULL;


    fd = open(nombreFichero, O_RDONLY);
    //En cas de que no poguem obrir el fitxer sortirem an un EXIT_FAILURE
    if (fd < 0) {
        write(1, ERR_OP_FILE, strlen(ERR_OP_FILE));
        exit(EXIT_FAILURE);
    }

    //Comprovem que el fitxer no estigui buit, en cas contrari sortirem amb un EXIT_FAILURE
    error = readDynamic(&lseat.client.nom, fd);
    if (error == 0) {
        write(1, ERR_EMPTY_FILE, strlen(ERR_EMPTY_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    } else if (error < 0) {
        write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
        close(fd);
        exit(EXIT_FAILURE);
    }

    //LLegim el saldo corresponent
    error = readDynamic(&cadena, fd);
    if (!error) {
        write(1, ERR_SALDO_FILE, strlen(ERR_SALDO_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    } else if (error < 0) {
        write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
        close(fd);
        exit(EXIT_FAILURE);
    } else {
        lseat.client.saldo = atoi(cadena);
        if (lseat.client.saldo == 0) {
            write(1, ERR_FORMAT_SALDO_FILE, strlen(ERR_FORMAT_SALDO_FILE));
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    //Llegim la IP del servidor al que ens tindrem que connectar
    error = readDynamic(&lseat.config.IP, fd);
    if (!error) {
        write(1, ERR_IP_FILE, strlen(ERR_IP_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    } else if (error < 0) {
        write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
        close(fd);
        exit(EXIT_FAILURE);
    }


    //LLegim el port al que ens connectarem al servidor
    error = readDynamic(&cadena, fd);
    if (!error) {
        write(1, ERR_PORT_FILE, strlen(ERR_PORT_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    } else if (error < 0) {
        write(1, ERROR_MEMORY, strlen(ERROR_MEMORY));
        close(fd);
        exit(EXIT_FAILURE);
    } else {
        lseat.config.Port = atoi(cadena);
        if (lseat.config.Port == 0) {
            write(1, ERR_FORMAT_PORT_FILE, strlen(ERR_FORMAT_PORT_FILE));
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (cadena != NULL) {
        free(cadena);
    }

    close(fd);
    return lseat;
}
