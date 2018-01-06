/**
 * @Author: Gabriel Cammany Ruiz
 * @Date:   8-12-2017
 * @Email:  ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: hash.h
 * @Last modified by:   Gabriel Cammany Ruiz
 * @Last modified time: 8-12-2017
 */


#ifndef LSEAT_MENUSTRUCTURE_H
#define LSEAT_MENUSTRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define ERR_MEM "Error en memoria, no s'ha pogut desar un nou element!\n"

typedef struct {

	char * key;
	int data;
	int number;

} mBucket;


typedef struct {

	int length;
	int elements;
	mBucket *bucket;

} Menu;

Menu MSTRUCTURE_createStructure(int size);

void MSTRUCTURE_insert(Menu *table, mBucket bucket);

void MSTRUCTURE_delete(Menu *table, char* key);

int MSTRUCTURE_findElement(Menu table, char* key);

int MSTRUCTURE_function(Menu table, char* key);

mBucket MSTRUCTURE_createBucket(char* key, int data, int number);

void MSTRUCTURE_destruct(Menu *table);

void MSTRUCTURE_incrementNum(Menu *table,int pos, int quantity);

int MSTRUCTURE_decrementNum(Menu *table,int pos, int quantity);

int MSTRUCTURE_getPrice(Menu table, int pos);

int MSTRUCTURE_isEmpty(Menu table);

void MSTRUCTURE_empty(Menu *table);

void MSTRUCTURE_returnCommands(Menu picard, Menu *enterprise);


#endif //LSEAT_MENUSTRUCTURE_H
