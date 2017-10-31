
#include <termios.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "shell.h"

struct termios saved_attributes;

void resetInput() {
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void setInputMode (void)
{
    struct termios tattr;

    /* Asegurarnos del terminal */
    if (!isatty (STDIN_FILENO))
    {
        //No es un terminal!
        exit (EXIT_FAILURE);
    }

    /* Desarem els atributs per poder restaurar el terminal en el seu estat inicial*/
    tcgetattr (STDIN_FILENO, &saved_attributes);

    /* Canvi dels atributs. */
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 1;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}


void readInput(char* buffer, char* menu) {

    int index = 0, max = 1;
    char c = ' ', aux[10];


    memset(buffer,0,150);
    buffer[index] = ' ';
    buffer[max] = ' ';
    buffer[max+1] = '\0';

    while (c != '\n'){

        read(0,&c,1);


        // is this an escape sequence?
        if (c == 27) {
            // "throw away" next two characters which specify escape sequence
            read(0,&c,1);
            read(0,&c,1);
            switch (c){
                case 'A': //Adalt
                    write(1,ADALT,strlen(ADALT));
                    break;
                case 'D': //Esquerra
                    if(index>0){
                        write(1,ESQUERRA,strlen(ESQUERRA));
                        index--;
                    }
                    break;
                case 'C': //Dreta
                    if(index<max-1){
                        write(1,DRETA,strlen(DRETA));
                        index++;
                    }
                    break;
                case 'B': //Abaix
                    write(1,ABAIX,strlen(ABAIX));
                    break;
                default:
                    break;
            }
            continue;
        }


        if (c == 0x7f) {

            if(index > 0){
                write(1,"\033[1D",strlen("\033[1D"));

                memmove(&buffer[index-1], &(buffer[index]),sizeof(char)*max);

                write(1,NETEJAR_LINIA,strlen(NETEJAR_LINIA));
                write(1,"\r",strlen("\r"));

                write (1, menu, strlen(menu));

                if(max > 0 && max >= index)max--;
                write(1,buffer,max);


                sprintf(aux,"\033[%dD",max-index+1);
                write(1,aux,strlen(aux));

                index--;

            }
            continue;
        }

        write(1,NETEJAR_LINIA,strlen(NETEJAR_LINIA));
        write(1,"\r",strlen("\r"));

        write (1, menu, strlen(menu));

        memmove(&buffer[index+1], &buffer[index],sizeof(char)*(max-index+1));

        buffer[index] = c;
        max++;

        write(1,buffer,max);

        sprintf(aux,"\033[%dD",max-index-1);
        write(1,aux,strlen(aux));


        index++;

    }
    buffer[index-1] = '\0';
}