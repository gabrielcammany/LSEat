//
// Created by gabriel on 8/12/17.
//

#include "../include/dataStructure.h"

#define EMPTY_BUCKET (-1)


int HASH_compareKeys(void* key, void* key2){
	return (*(int*)key == *(int*)key2);
}

int HASH_isPosEmpty(void* key){
	return *(int*)key == EMPTY_BUCKET;
}


void HASH_deleteBucket(Bucket *bucket){

	bucket->key = EMPTY_BUCKET;
	free(bucket->data);
	bucket->number = 0;

}

Table HASH_createTable(int size) {
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

		}

	}
	return table;

}

void HASH_checkAndRestructure(Table *table) {
	int i = 0;
	Table aux;

	if (table->elements == table->length) {

		aux = HASH_createTable(table->length * 2);

		if (aux.length != -1) {

			for (i = 0; i < table->length; i++) {

				HASH_insert(&aux, table->bucket[i]);

			}

			table->bucket = aux.bucket;
			table->number = aux.number;
			table->length = aux.length;
			table->elements = aux.elements;

		}
	}

}

void HASH_insert(Table *table, Bucket bucket) {

	int pos = 0;

	pos = HASH_function(*table, bucket.key);

	if (!HASH_compareKeys(&table->bucket[pos].key,&bucket.key)) {

		while (!HASH_isPosEmpty(&table->bucket[pos])) {

			if (HASH_compareKeys(&table->bucket[pos].key,&bucket.key)){break;}

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (!HASH_compareKeys(&table->bucket[pos].key,&bucket.key)) {

			table->elements++;

			HASH_checkAndRestructure(table);

		}

	}

	table->bucket[pos].key = bucket.key;
	if(bucket.data != NULL)table->bucket[pos].data = bucket.data;
	table->bucket[pos].number = bucket.number;

	if (table->bucket[pos].number <= table->bucket[table->number].number) {

		table->number = pos;

	}

}


int HASH_delete(Table *table, int key) {
	int pos;

	pos = HASH_function(*table, key);

	if (!HASH_compareKeys(&table->bucket[pos].key,&key)) {

		while (!HASH_compareKeys(&table->bucket[pos].key,&key) && !HASH_isPosEmpty(&table->bucket[pos].key)) {

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (HASH_compareKeys(&table->bucket[pos].key,&key)) {

			HASH_deleteBucket(&table->bucket[pos]);

		}else{

			return -1;

		}

	} else {

		HASH_deleteBucket(&table->bucket[pos]);

	}

	return 1;

}


int HASH_findElement(Table table, int key) {
	int pos = 0;

	pos = HASH_function(table, key);

	while (!HASH_compareKeys(&table.bucket[pos].key,&key) && !HASH_isPosEmpty(&table.bucket[pos].key)) {

		pos++;

		if (pos == table.length) {
			pos = 0;
		}

	}

	return (!HASH_isPosEmpty(&table.bucket[pos].key) ? pos : EMPTY_BUCKET);

}

int HASH_function(Table table, int key) {
	return key % (table.length - 1);
}

void HASH_destruct(Table *table){
	int i;

	if(table->bucket!= NULL){

		for (i = 0; i < table->elements; i++) {
			if(table->bucket[i].data != NULL)free(table->bucket[i].data);
		}

		free(table->bucket);
	}


}



Bucket HASH_createBucket(int key, char *data, int number){
	Bucket bucket;

	bucket.key = key;

	if(data != NULL){
		bucket.data = (char*) calloc(strlen(data),strlen(data) *  sizeof(char));

		if(bucket.data != NULL){

			memcpy(bucket.data,data,strlen(data)*sizeof(char));

		}
	}else{

		bucket.data = NULL;

	}

	bucket.number = number;

	return bucket;
}
