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

}Bucket;


typedef struct {

	int length;
	int elements;
	Bucket *bucket;
	int number;

}Table;
/**
 * Creates the hash table
 * @param size Hash table size
 * @return returns the hash table
 */
Table HASH_createTable(int size);
/**
 * Function to instert new Enterprise in the hash table
 * @param table Hash Table
 * @param bucket Element to insert
 */
void HASH_insert(Table *table, Bucket bucket);
/**
 * Function to delete one element from its key
 * @param table Hash Table
 * @param key Plate Name
 */
int HASH_delete(Table *table, int key);

int HASH_deletePos(Table *table,int pos);
/**
 * Searchs for the element that has that key
 * @param table Hash Table
 * @param key Plate name
 * @return returns position, if not -1
 */
int HASH_findElement(Table table, int key);
/**
 * Function that uses findElement to search for the Elemet
 * @param table Hash Table
 * @param key plate name
 * @return returns value form key
 */
int HASH_function(Table table, int key);
/**
 * Create element of hash table
 * @param key plate name
 * @param data string
 * @param number picards
 * @return returns the element
 */
Bucket HASH_createBucket(int key, char *data, int number);
/**
 * Free all memory used by the hash table
 * @param table Hash Table
 */
void HASH_destruct(Table *table);

void HASH_deleteBucket(Bucket *bucket);

#endif //LSEAT_DATASTRUCTURE_H
