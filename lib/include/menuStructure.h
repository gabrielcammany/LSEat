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

	char *key;
	int data;
	int number;

} mBucket;


typedef struct {

	int length;
	int elements;
	mBucket *bucket;

} Menu;

/**
 * Creates the hash table
 * @param size Hash table size
 * @return returns the hash table
 */
Menu MSTRUCTURE_createStructure(int size);

/**
 * Function to instert new plate in the hash table
 * @param table Hash Table
 * @param bucket Element to insert
 */
void MSTRUCTURE_insert(Menu *table, mBucket bucket);

/**
 * Function to delete one element from its key
 * @param table Hash Table
 * @param key Plate Name
 */
void MSTRUCTURE_delete(Menu *table, char *key);

/**
 * Searchs for the element that has that key
 * @param table Hash Table
 * @param key Plate name
 * @return returns position, if not -1
 */
int MSTRUCTURE_findElement(Menu table, char *key);

/**
 * Function that uses findElement to search for the Elemet
 * @param table Hash Table
 * @param key plate name
 * @return returns value form key
 */
int MSTRUCTURE_function(Menu table, char *key);

/**
 * Create element of hash table
 * @param key plate name
 * @param data price
 * @param number stock
 * @return returns the element
 */
mBucket MSTRUCTURE_createBucket(char *key, int data, int number);

/**
 * Free all memory used by the hash table
 * @param table Hash Table
 */
void MSTRUCTURE_destruct(Menu *table);

/**
 *Increments the stock by quantity of the element in pos
 * @param table Hash Table
 * @param pos Position of hash Table
 * @param quantity Units to increment
 */
void MSTRUCTURE_incrementNum(Menu *table, int pos, int quantity);

/**
 *Decrements the stock by quantity of the element in pos
 * @param table Hash Table
 * @param pos Position of hash Table
 * @param quantity Units to increment
 */
int MSTRUCTURE_decrementNum(Menu *table, int pos, int quantity);

/**
 * Calculates the price of the plate
 * @param table Hash Table
 * @param pos Position of the Hash Table
 * @return
 */
int MSTRUCTURE_getPrice(Menu table, int pos);

/**
 * Tell us if the hash table
 * @param table
 * @return
 */
int MSTRUCTURE_isEmpty(Menu table);

/**
 * Empties the hash table
 * @param table
 */
void MSTRUCTURE_empty(Menu *table);

/**
 * Returns the commands from picard to enterprise
 * @param picard Hash table of commands
 * @param enterprise Menu hash table
 */
void MSTRUCTURE_returnCommands(Menu picard, Menu *enterprise);


#endif //LSEAT_MENUSTRUCTURE_H
