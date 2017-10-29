/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: types.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 26-10-2017
 */

#ifndef _TYPES_H_
#define _TYPES_H_

/*
 * Typedef del client on per ara tindrem el saldo i el nom.
 */
typedef struct{
    int saldo;
    char *nom;
}Client;

/*
 * Typedef de la configuració on guardarem el port i la IP del servidor
 */
typedef struct{
    char *IP;
    int Port;
}Config;

/*
 * Typedef principal
 */
typedef struct{
    Config config;
    Client client;
}LSEat;

#endif
