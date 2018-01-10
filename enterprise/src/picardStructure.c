//
// Created by gabriel on 8/12/17.
//

#include <pthread.h>
#include <signal.h>
#include "../include/picardStructure.h"

#define EMPTY_BUCKET (-1)
#define MIN_FD 2


int PSTRUCTURE_compareKeys(void* key, void* key2){
	return (*(int*)key == *(int*)key2);
}

int PSTRUCTURE_isPosEmpty(void* key){
	return *(int*)key == EMPTY_BUCKET;
}


void PSTRUCTURE_deleteBucket(Bucket *bucket){

	if(bucket->key > MIN_FD){
		close(bucket->key);
		bucket->key = EMPTY_BUCKET;
	}

	MSTRUCTURE_destruct(&bucket->commanda);

	free(bucket->data);

	bucket->number = 0;

}

Table PSTRUCTURE_createTable(int size) {
	Table table;
	int i;
	table.bucket = NULL;

	table.length = size;
	table.bucket = (Bucket *) calloc(size,sizeof(Bucket) * size);
	table.elements = 0;
	table.number = 0;

	if (table.bucket == NULL) {

		write(1, ERR_MEM, strlen(ERR_MEM));
		table.length = -1;

	}else{

		for( i = 0; i < size; i++){

			table.bucket[i].data = NULL;
			table.bucket[i].number = 0;
			table.bucket[i].key = EMPTY_BUCKET;
			table.bucket[i].pthread = 0;

		}

	}
	return table;

}

void PSTRUCTURE_checkAndRestructure(Table *table) {
	int i = 0;
	Table aux;

	if (table->elements == table->length) {

		aux = PSTRUCTURE_createTable(table->length * 2);

		if (aux.length != -1) {

			for (i = 0; i < table->length; i++) {

				PSTRUCTURE_insert(&aux, table->bucket[i]);

			}

			table->bucket = aux.bucket;
			table->number = aux.number;
			table->length = aux.length;
			table->elements = aux.elements;

		}
	}

}

void PSTRUCTURE_insert(Table *table, Bucket bucket) {

	int pos = 0;

	pos = PSTRUCTURE_function(*table, bucket.key);

	if (!PSTRUCTURE_compareKeys(&table->bucket[pos].key,&bucket.key)) {

		while (!PSTRUCTURE_isPosEmpty(&table->bucket[pos])) {

			if (PSTRUCTURE_compareKeys(&table->bucket[pos].key,&bucket.key)){break;}

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (!PSTRUCTURE_compareKeys(&table->bucket[pos].key,&bucket.key)) {

			table->elements++;

			PSTRUCTURE_checkAndRestructure(table);

		}

	}

	table->bucket[pos].key = bucket.key;
	if(bucket.data != NULL)table->bucket[pos].data = bucket.data;
	table->bucket[pos].number = bucket.number;
	table->bucket[pos].pthread = bucket.pthread;
	table->bucket[pos].commanda = bucket.commanda;

	if (table->bucket[pos].number <= table->bucket[table->number].number) {

		table->number = pos;

	}

}

int PSTRUCTURE_deletePos(Table *table, int pos){

	if(pos < table->length && pos > -1){
		PSTRUCTURE_deleteBucket(&table->bucket[pos]);

		table->elements--;

		return 1;

	}else{

		return 0;

	}

}

int PSTRUCTURE_delete(Table *table, int key) {
	int pos;

	pos = PSTRUCTURE_function(*table, key);

	if (!PSTRUCTURE_compareKeys(&table->bucket[pos].key,&key)) {

		while (!PSTRUCTURE_compareKeys(&table->bucket[pos].key,&key) && !PSTRUCTURE_isPosEmpty(&table->bucket[pos].key)) {

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (PSTRUCTURE_compareKeys(&table->bucket[pos].key,&key)) {

			PSTRUCTURE_deleteBucket(&table->bucket[pos]);

			if(table->bucket[pos].pthread > 0){
				pthread_kill(table->bucket[pos].pthread, SIGUSR2);
				pthread_join(table->bucket[pos].pthread, NULL);
			}

		}else{

			return -1;

		}

	} else {

		PSTRUCTURE_deleteBucket(&table->bucket[pos]);

		if(table->bucket[pos].pthread > 0){
			pthread_kill(table->bucket[pos].pthread, SIGUSR2);
			pthread_join(table->bucket[pos].pthread, NULL);
		}

	}

	return 1;

}


int PSTRUCTURE_findElement(Table table, int key) {
	int pos = 0;

	//We get the position in the table of this picard through the key
	pos = PSTRUCTURE_function(table, key);

	//A simple function that returns if the keys are equal and if the position
	//we are looking at is empty
	while (!PSTRUCTURE_compareKeys(&table.bucket[pos].key,&key) && !PSTRUCTURE_isPosEmpty(&table.bucket[pos].key)) {
		//In case is true we are going to move linely
		pos++;

		if (pos == table.length) {
			pos = 0;
		}

	}

	//If the client is inside of our table then we return it's position, if not -1 (EMPTY_BUCKET)
	return (!PSTRUCTURE_isPosEmpty(&table.bucket[pos].key) ? pos : EMPTY_BUCKET);

}

int PSTRUCTURE_function(Table table, int key) {
	return key % (table.length - 1);
}

void PSTRUCTURE_destruct(Table *table){
	int i;

	if(table->bucket != NULL){

		for (i = 0; i < table->length && table->elements > 0; i++) {

			if (table->bucket[i].key != EMPTY_BUCKET){

				PSTRUCTURE_deleteBucket(&table->bucket[i]);

				table->elements--;

			}
			if(table->bucket[i].pthread > 0){
				pthread_kill(table->bucket[i].pthread, SIGKILL);
				pthread_join(table->bucket[i].pthread, NULL);
			}
		}

		free(table->bucket);
	}


}


Bucket PSTRUCTURE_createBucket(int key, char *data, int number, pthread_t pthread ){
	Bucket bucket;

	bucket.key = key;

	bucket.data = (char*) calloc(strlen(data),strlen(data) *  sizeof(char));

	if(bucket.data != NULL){

		memcpy(bucket.data,data,strlen(data)*sizeof(char));
		bucket.number = number;
		bucket.pthread = pthread;
		bucket.commanda = MSTRUCTURE_createStructure(MIN_COMMAND);

	}

	return bucket;
}

void PSTRUCTURE_calculateMoneyLeft(int option, Bucket *bucket, int price, int units) {

	switch(option){
		case 1:
			bucket->number += (price*units);
			break;
		case 2:
			bucket->number -= (price*units);
			break;
		default:
			break;
	}
}
