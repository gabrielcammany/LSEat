/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   8-12-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: hash.h
 * @Last modified by:   Gabriel Cammany Ruiz
 * @Last modified time: 8-12-2017
 */


#ifndef LSEAT_DATASTRUCTURE_H
#define LSEAT_DATASTRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define ERR_MEM "Error en memoria, no s'ha pogut desar un nou element!\n"


typedef struct {

	int key;
	char* data;
	int number;

} Bucket;


typedef struct {

	int length;
	int elements;
	Bucket *bucket;
	int number;

} Table;

Table HASH_createTable(int size);

void HASH_insert(Table *table, Bucket bucket);

int HASH_delete(Table *table, int key);

int HASH_findElement(Table table, int key);

int HASH_function(Table table, int key);

Bucket HASH_createBucket(int key, char *data, int number);

void HASH_destruct(Table *table);


#endif //LSEAT_DATASTRUCTURE_H
