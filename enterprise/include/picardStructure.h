/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   8-12-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: hash.h
 * @Last modified by:   Gabriel Cammany Ruiz
 * @Last modified time: 8-12-2017
 */


#ifndef LSEAT_PICARDSTRUCTURE_H_
#define LSEAT_PICARDSTRUCTURE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "../../lib/include/menuStructure.h"


#define ERR_MEM "Error en memoria, no s'ha pogut desar un nou element!\n"
#define MIN_COMMAND 5

typedef struct {

	int key;
	char* data;
	int number;
	pthread_t pthread;
	Menu commanda;

} Bucket;

typedef struct {

	int length;
	int elements;
	Bucket *bucket;
	int number;

} Table;

Table PSTRUCTURE_createTable(int size);

void PSTRUCTURE_insert(Table *table, Bucket bucket);

int PSTRUCTURE_delete(Table *table, int key);

int PSTRUCTURE_findElement(Table table, int key);

int PSTRUCTURE_function(Table table, int key);

Bucket PSTRUCTURE_createBucket(int key, char *data, int number, pthread_t pthread );

void PSTRUCTURE_destruct(Table *table);

void PSTRUCTURE_deleteBucket(Bucket *bucket);

void PSTRUCTURE_calculateMoneyLeft(int option, Bucket *bucket, int price, int units);



#endif //LSEAT_PICARDSTRUCTURE_H_
