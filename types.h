/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: types.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 25-10-2017
 */

#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct{
    int saldo;
    char *nom;
}Client;

typedef struct{
    char *IP;
    int Port;
}Config;

typedef struct{
    Config config;
    Client client;
}LSEat;

#endif
