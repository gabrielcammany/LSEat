/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 25-10-2017
 */
#include "shell.h"

void Shell(LSEat lseat) {

    char cadena[50];
    char *input = NULL;
    int final = 0;

    write (1, INTRODUCTION, strlen(INTRODUCTION));
    sprintf(cadena, CLIENT, lseat.client.nom);

    while ( !final ) {
        write (1, cadena, strlen(cadena));
        readDynamic(&input, 0);
        final = ManageShell(input);
    }

}


int ManageShell(char *input) {
    int i= 0;

    while( input[i]) {
        input[i] = tolower(input[i]);
        i++;
    }

    if(strcmp(input,"connecta") == 0){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if(strcmp(input,"mostra menu") == 0){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if(strcmp(input,"demana") == 0){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if(strcmp(input,"elimina") == 0){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if(strcmp(input,"pagar") == 0){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if(strcmp(input,"desconnecta") == 0){
        write(1, BYE, strlen(BYE));
        return 1;
    }else{
        write (1, COMMANDA_KO, strlen(COMMANDA_KO));

    }
    
    return 0;
}
