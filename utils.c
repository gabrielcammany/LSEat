/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: utils.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "utils.h"

 /**
 Function that reads buffer specified in variable fd, and loads the data to
 the input char*
 **/
 int readDynamic(char** input, int fd) {
     char* temporal = NULL;
     char buffer = ' ';
     int indice = 0;

     while (read (fd, &buffer, sizeof(char)) > 0 && buffer != '\n') {
         temporal = (char*)realloc(*input,sizeof(char)*(indice+1));

         if (temporal == NULL) {

             if(*input != NULL){
                 free(*input);
             }
             return -1;

         }

         *input = temporal;
         (*input)[indice] = buffer;

         indice++;
     }

     if(*input != NULL){
         temporal = (char*)realloc(*input,sizeof(char)*(indice+1));
         if(temporal == NULL){

             free(*input);
             return -1;

         }

         *input = temporal;
         (*input)[indice] = '\0';
     }

     return indice;
 }


 int getArrayString(char *input, char delimiter, int *espais){
     int index = 0;
     int buida = 0;

     for(*espais = 0; input[*espais] == ' ';(*espais)++){}

     for(index = *espais; input[index] != delimiter && input[index] != '\0'; index++){

         if(input[index] != ' '){
             buida = 1;
         }
     }

     if( buida == 0){
         return -1;
     }

     return index;
 }
