/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   25-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: shell.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 30-10-2017
 */

#ifndef _SHELL_H_
#define _SHELL_H_

#define ADALT "\033[1A"
#define ABAIX "\033[1B"
#define DRETA "\033[1C"
#define ESQUERRA "\033[1D"
#define NETEJAR_LINIA "\033[2K"

/**
 * Establece el modo de input
 */
void setInputMode();

/**
 * Resetea el input que hemos modificado
 */
void resetInput();

/**
 * Funcion que se encarga de gestionar el input del usuario
 * @param buffer Estructura de datos donde se vuelca los datos introducidos
 * @param menu Cadena de caracters que incluye el prompt
 */
void readInput(char *buffer, char *menu);

#endif
