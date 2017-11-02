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
 * Typedef del cliente que contiene su informacion
 */
typedef struct {
	int saldo;
	char *nom;
} Client;

/*
 * Typedef de un enterprise
 */
typedef struct {
	int refresh;
	char *nom;
} Enterprise;

/*
 * Typedef de configuracion de red
 */
typedef struct {
	char *IP;
	int Port;
} Config;


/*
 * Typedef del cliente base
 */

typedef struct {
	Config config;
	Client client;
} ClientLSEat;

/*
 * Typedef del enterprise base
 */
typedef struct {
	Config picards;
	Config data;
	Enterprise enterprise;
} EnterpriseLSEat;

/*
 * Typedef del data base
 */
typedef struct {
	Config config;
	int p_enterprise;
} DataLSEat;



#endif
