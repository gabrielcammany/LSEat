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
	Menu comanda;

} Bucket;

typedef struct {

	int length;
	int elements;
	Bucket *bucket;
	int number;

} Table;
/**
 * Creates the hash table
 * @param size Hash table size
 * @return returns the hash table
 */
Table PSTRUCTURE_createTable(int size);
/**
 * Function to instert new Picard in the hash table
 * @param table Hash Table
 * @param bucket Element to insert
 */
void PSTRUCTURE_insert(Table *table, Bucket bucket);
/**
 * Function to delete one element from its key
 * @param table Hash Table
 * @param key Plate Name
 */
int PSTRUCTURE_delete(Table *table, int key);

int PSTRUCTURE_deletePos(Table *table, int pos);
/**
 * Searchs for the element that has that key
 * @param table Hash Table
 * @param key Plate name
 * @return returns position, if not -1
 */
int PSTRUCTURE_findElement(Table table, int key);
/**
 * Function that uses findElement to search for the Elemet
 * @param table Hash Table
 * @param key plate name
 * @return returns value form key
 */
int PSTRUCTURE_function(Table table, int key);
/**
 * Create element of hash table
 * @param key plate name
 * @param data price
 * @param number stock
 * @return returns the element
 */
Bucket PSTRUCTURE_createBucket(int key, char *data, int number, pthread_t pthread );

/**
 * Free all memory used by the hash table
 * @param table Hash Table
 */
void PSTRUCTURE_destruct(Table *table);

void PSTRUCTURE_deleteBucket(Bucket *bucket);

void PSTRUCTURE_calculateMoneyLeft(int option, Bucket *bucket, int price, int units);



#endif //LSEAT_PICARDSTRUCTURE_H_
