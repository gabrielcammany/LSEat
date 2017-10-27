/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */
#include "shell.h"

void Shell(LSEat lseat) {

    char cadena[50];
    char *input = NULL;
    int final = 0;

    write (1, INTRODUCTION, strlen(INTRODUCTION));

    while ( !final ) {
        sprintf(cadena, "%s\t", lseat.client.nom);
        write (1, cadena, strlen(cadena));
        write (1, ">\t", strlen(">"));
        readDynamic(&input, 0);
        final = ManageShell(input);
    }

    free(input);

}


int ManageShell(char *input) {
    int i= 0;

    while( input[i]) {
        input[i] = tolower(input[i]);
        i++;
    }

    if(strcmp( input,"connecta") == 0 ){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if( strcmp(input,"mostra menu") == 0 ){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if( strcmp(input,"pagar") == 0 ){
        write(1, COMMANDA_OK, strlen(COMMANDA_OK));

    }else if( strcmp(input,"desconnecta") == 0 ){
        write(1, BYE, strlen(BYE));
        return 1;

    }else{
        SpecialCommand(input);

    }

    return 0;
}

void SpecialCommand(char *input) {

    char* string = NULL;

    string = getArrayString(input,0,' ');
    if(string == NULL) {
        write(1, COMMANDA_KO, strlen(COMMANDA_KO));

    }else {

        if ( strcmp(string, "elimina") == 0 ) {
            CheckSpecialCommand(input,string);
            free(string);
            
        }else if ( strcmp (string, "demana") == 0) {
            CheckSpecialCommand(input,string);
            free(string);

        } else {
            write(1, COMMANDA_KO, strlen(COMMANDA_KO));

        }
    }
}

void CheckSpecialCommand(char *input, char *string) {
    int num_plats = 0;

    string = getArrayString(input, ((int)strlen(string))+1, ' ');

    if(string == NULL) {
        write(1, COMMANDA_KO, strlen(COMMANDA_KO));

    } else {
        num_plats = atoi(string);

        if ( num_plats <= 0 ){
            write(1, COMMANDA_KO, strlen(COMMANDA_KO));

        } else {
            string = getArrayString(input, ((int)strlen(string))+1, '\0');

            if(string == NULL) {
                write(1, COMMANDA_KO, strlen(COMMANDA_KO));
            } else {
                write(1, COMMANDA_OK, strlen(COMMANDA_OK));

            }
        }
    }

}
