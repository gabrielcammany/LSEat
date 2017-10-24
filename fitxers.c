/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 25-10-2017
 */


#include "fitxers.h"

LSEat LecturaFitxerConfigClient(char nomFitxer[]) {

    int fd = 0, llegir = 0, index = 0, index_aux = 0;
    char c;
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

    llegir = read (fd, &c, 1);
    //Comprovem que el fitxer no estigui buit, en cas contrari sortirem amb un EXIT_FAILURE
    if( llegir == 0) {
        write(1, ERR_EMPTY_FILE, strlen(ERR_EMPTY_FILE));
        close(fd);
        exit(EXIT_FAILURE);
    }

    //LLegim el Nom del client que s'está conectant al servidor
    while ( c != '\n' ) {
        lseat.client.nom = (char*) realloc (lseat.client.nom, sizeof(char)*(index+1));
        lseat.client.nom[index_aux] = c;
        index_aux++;
        index++;
        llegir = read (fd, &c, 1);
    }

    lseat.client.nom[index_aux] = '\0';
    index = 0;
    index_aux = 0;

    //LLegim el saldo corresponent
    llegir = read (fd, &c, 1);

    while ( c != '\n' ) {
        cadena = (char*) realloc (cadena, sizeof(char)*(index+1));
        cadena[index_aux] = c;
        index_aux++;
        index++;
        llegir = read (fd, &c, 1);
    }

    cadena[index_aux] = '\0';
    lseat.client.saldo = atoi(cadena);
    index = 0;
    index_aux = 0;

    //Comprovem que tot ha anat bé i per tant alliverem memoria.
    if( cadena != NULL) {
        free(cadena);
        cadena = NULL;
    }


    llegir = read (fd, &c, 1);
    //Llegim la IP del servidor al que ens tindrem que connectar
    while ( c != '\n' ) {
        lseat.config.IP = (char*) realloc (lseat.config.IP, sizeof(char)*(index+1));
        lseat.config.IP[index_aux] = c;
        index_aux++;
        index++;
        llegir = read (fd, &c, 1);
    }

    lseat.config.IP[index_aux] = '\0';
    index = 0;
    index_aux = 0;

    //LLegim el port al que ens connectarem al servidor
    llegir = read (fd, &c, 1);

    while ( c != '\n' || llegir != 0) {
        cadena = (char*) realloc (cadena, sizeof(char)*(index+1));
        cadena[index_aux] = c;
        index_aux++;
        index++;
        llegir = read (fd, &c, 1);
    }

    cadena[index_aux] = '\0';
    lseat.config.Port = atoi(cadena);
    index = 0;
    index_aux = 0;

    //Comprovem que tot ha anat bé i per tant alliverem memoria.
    if( cadena != NULL) {
        free(cadena);
        cadena = NULL;
    }

    close(fd);
    return lseat;
}
