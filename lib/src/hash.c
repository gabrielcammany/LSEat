//
// Created by gabriel on 8/12/17.
//


#include "../include/hash.h"

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
			table.bucket[i].key = EMPTY_BUCKET;
			table.bucket[i].number = 0;

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

	if (table->bucket[pos].key != bucket.key) {

		while (table->bucket[pos].key != EMPTY_BUCKET) {

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

			if (table->bucket[pos].key == bucket.key) {break;}


		}

		if (table->bucket[pos].key != bucket.key) {

			table->bucket[pos].key = bucket.key;
			table->bucket[pos].data = bucket.data;
			table->bucket[pos].number = 0;

			table->elements++;

			HASH_checkAndRestructure(table);

		}else{

			if(bucket.data != NULL)table->bucket[pos].data = bucket.data;
			table->bucket[pos].number = bucket.number;

		}

	} else {

		if(bucket.data != NULL)table->bucket[pos].data = bucket.data;
		table->bucket[pos].number = bucket.number;

	}

	if(table->bucket[pos].number <= table->bucket[table->number].number){

		table->number = pos;

	}

}


void HASH_delete(Table *table, int key) {
	int pos;

	pos = HASH_function(*table, key);

	if (table->bucket[pos].key != key) {


		while (table->bucket[pos].key != key && table->bucket[pos].key != EMPTY_BUCKET) {

			pos++;

			if (pos == table->length) {
				pos = 0;
			}

		}

		if (table->bucket[pos].key == key) {

			table->bucket[pos].key = EMPTY_BUCKET;
			if (table->bucket[pos].data != NULL)free(table->bucket[pos].data);

		}

	} else {

		table->bucket[pos].key = EMPTY_BUCKET;
		if (table->bucket[pos].data != NULL)free(table->bucket[pos].data);

	}

}


int HASH_findElement(Table table, int key) {
	int pos = 0;

	pos = HASH_function(table, key);

	while (table.bucket[pos].key != key && table.bucket[pos].key != EMPTY_BUCKET) {

		pos++;

		if (pos == table.length) {
			pos = 0;
		}

	}

	printf("Posicio: %d\n", pos);
	printf("Posicio: %d\n", table.bucket[pos].key);

	return (table.bucket[pos].key != EMPTY_BUCKET ? pos : EMPTY_BUCKET);

}

int HASH_function(Table table, int key) {
	return key % (table.length - 1);
}

Bucket HASH_createBucket(int key, char *data, int number) {
	Bucket bucket;

	bucket.key = key;
	bucket.data = data;
	bucket.number = number;

	return bucket;
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