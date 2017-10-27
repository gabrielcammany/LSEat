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

LSEat LecturaFitxerConfigClient(char nomFitxer[]) {

    int fd = 0;
    char *cadena = NULL;
    LSEat lseat;

    lseat.config.IP = NULL;
    lseat.client.nom = NULL;


    fd = open (nomFitxer, O_RDONLY);
    //En cas de que no poguem obrir el fitxer sortirem an un EXIT_FAILURE
    if( fd < 0 ){
        write(1, ERR_OP_FILE, strlen(ERR_OP_FILE));
        exit(EXIT_FAILURE);
    }

    //Comprovem que el fitxer no estigui buit, en cas contrari sortirem amb un EXIT_FAILURE
    if( readDynamic(&lseat.client.nom,fd) < 0) {
        write(1, ERR_EMPTY_FILE, strlen(ERR_EMPTY_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    }

    //LLegim el saldo corresponent i alliberem memoria de cadena un cop utilitzada
    readDynamic(&cadena,fd);
    lseat.client.saldo = atoi(cadena);

    if( cadena != NULL) {
        free(cadena);
        cadena = NULL;
    }

    //Llegim la IP del servidor al que ens tindrem que connectar
    readDynamic(&lseat.config.IP,fd);


    //LLegim el port al que ens connectarem al servidor
    //Lliberem memoria en cas de que hagi funcionat bé
    readDynamic(&cadena,fd);
    lseat.config.Port = atoi(cadena);

    if( cadena != NULL) {
        free(cadena);
        cadena = NULL;
    }

    close(fd);
    return lseat;
}
