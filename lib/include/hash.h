/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   8-12-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: hash.h
 * @Last modified by:   Gabriel Cammany Ruiz
 * @Last modified time: 8-12-2017
 */


#ifndef LSEAT_HASH_H
#define LSEAT_HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define ERR_MEM "Error en memoria, no s'ha pogut desar un nou element!\n"

#define EMPTY_BUCKET (-1)

typedef struct {

	int key;
	void *data;
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

void HASH_delete(Table *table, int key);

int HASH_findElement(Table table, int key);

int HASH_function(Table table, int key);

Bucket HASH_createBucket(int key, void *data, int number);

#endif //LSEAT_HASH_H
