//
// Created by gabriel on 8/12/17.
//


#include <limits.h>
#include "../include/menuStructure.h"

#define EMPTY_BUCKET (NULL)

Menu MSTRUCTURE_createStructure(int size) {
	Menu table;
	int i;
	table.bucket = NULL;

	table.length = size;
	table.bucket = (mBucket *) calloc( (size_t) size, sizeof(mBucket) * size);
	table.elements = 0;

	if (table.bucket == NULL) {

		write(1, ERR_MEM, strlen(ERR_MEM));
		table.length = -1;

	} else {

		for (i = 0; i < size; i++) {

			table.bucket[i].data = 0;
			table.bucket[i].key = EMPTY_BUCKET;

		}

	}

	return table;

}

void MSTRUCTURE_checkAndRestructure(Menu *table) {
	int i = 0;
	Menu aux;

	if (table->elements == table->length) {

		aux = MSTRUCTURE_createStructure(table->length * 2);

		if (aux.length != -1) {

			for (i = 0; i < table->length; i++) {

				MSTRUCTURE_insert(&aux, table->bucket[i]);

			}

			table->bucket = aux.bucket;
			table->length = aux.length;
			table->elements = aux.elements;

		}
	}

}


void MSTRUCTURE_insert(Menu *table, mBucket bucket) {

	int pos = 0;


	pos = MSTRUCTURE_function(*table, bucket.key);

	if(table->bucket[pos].key != NULL){
		if (strcmp(table->bucket[pos].key,bucket.key) != 0) {

			while (table->bucket[pos].key != EMPTY_BUCKET) {

				if (strcmp(table->bucket[pos].key,bucket.key) == 0) { break; }

				pos++;

				if (pos == table->length) {
					pos = 0;
				}

			}

			if (table->bucket[pos].key == EMPTY_BUCKET) {

				table->bucket[pos].key = bucket.key;
				table->bucket[pos].data = bucket.data;
				table->bucket[pos].number = bucket.number;

				table->elements++;

				MSTRUCTURE_checkAndRestructure(table);

			} else {

				table->bucket[pos].data = bucket.data;
				table->bucket[pos].number = bucket.number;

			}

		} else {

			table->bucket[pos].data = bucket.data;
			table->bucket[pos].number = bucket.number;

		}
	}else{

		table->bucket[pos].key = bucket.key;
		table->bucket[pos].data = bucket.data;
		table->bucket[pos].number = bucket.number;

		table->elements++;

		MSTRUCTURE_checkAndRestructure(table);

	}


}


void MSTRUCTURE_delete(Menu *table, char *key) {
	int pos;

	pos = MSTRUCTURE_function(*table, key);

	if (strcmp(table->bucket[pos].key, key) != 0) {


		while (!strcmp(table->bucket[pos].key, key) && table->bucket[pos].key != EMPTY_BUCKET) {

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (!strcmp(table->bucket[pos].key, key)) {

			if (table->bucket[pos].key != NULL){
				free(table->bucket[pos].key);
				table->bucket[pos].key = EMPTY_BUCKET;
				table->bucket[pos].number = 0;
				table->elements--;
			}

		}

	} else {

		if (table->bucket[pos].key != NULL){
			free(table->bucket[pos].key);
			table->bucket[pos].key = EMPTY_BUCKET;
			table->bucket[pos].number = 0;
			table->elements--;
		}

	}

}


int MSTRUCTURE_findElement(Menu table, char *key) {
	int pos = 0;

	pos = MSTRUCTURE_function(table, key);

	while (table.bucket[pos].key != EMPTY_BUCKET && strcmp(table.bucket[pos].key, key) != 0) {
		//printf("KEEEY: %s\n",table.bucket[pos].key);
		pos++;

		if (pos == table.length) {
			pos = 0;
		}

	}
	//printf("OUT KEEEY: %s\n",table.bucket[pos].key);
	return (table.bucket[pos].key != EMPTY_BUCKET ? pos : -1);

}

int MSTRUCTURE_function(Menu table, char *key) {
	unsigned long int hashval = 0;
	int i = 0;

	while( hashval < ULONG_MAX && i < (int) strlen(key) ) {
		hashval = hashval << 8;
		hashval += key[i];
		i++;
	}

	return (int) hashval % table.length;
}



mBucket MSTRUCTURE_createBucket(char *key, int data, int number) {
	mBucket bucket;

	//bucket.key = key;

	bucket.key = (char*) calloc(strlen(key),strlen(key) *  sizeof(char));

	if(bucket.key != NULL){

		memcpy(bucket.key,key,strlen(key)*sizeof(char));
		bucket.data = data;
		bucket.number = number;

	}

	return bucket;
}

void MSTRUCTURE_destruct(Menu *table) {
	int i;

	if (table->bucket != NULL) {

		for (i = 0; i < table->length && table->elements > 0; i++) {

			if (table->bucket[i].key != NULL){
				free(table->bucket[i].key);
				table->elements--;
			}

		}

		if(table->bucket != NULL){
			free(table->bucket);
			table->bucket = NULL;
		}
	}


}


void MSTRUCTURE_incrementNum(Menu *table,int pos, int quantity){
	table->bucket[pos].number += quantity;

}

int MSTRUCTURE_decrementNum(Menu *table,int pos, int quantity){


	if((table->bucket[pos].number - quantity) >= 0){
		table->bucket[pos].number -= quantity;
		return 1;
	}else{
		return -1;
	}
}

int MSTRUCTURE_getPrice(Menu table, int pos){
	return table.bucket[pos].data;
}

int MSTRUCTURE_isEmpty(Menu table) {
	return table.elements;
}

void MSTRUCTURE_empty(Menu *table){
	int i = 0;

	while(table->elements != 0) {
		if (table->bucket[i].key != EMPTY_BUCKET) {
			MSTRUCTURE_delete(table,table->bucket[i].key);
		}
		i++;
	}
}


void MSTRUCTURE_returnCommands(Menu picard, Menu *enterprise){
	int i = 0;
	int num = 0, numP = 0;

	while( numP < picard.elements ){

		if(picard.bucket[i].key != EMPTY_BUCKET) {
			num = MSTRUCTURE_findElement(*enterprise,picard.bucket[i].key);
			enterprise->bucket[num].number += picard.bucket[i].number;
			numP++;
		}
		i++;
	}
}