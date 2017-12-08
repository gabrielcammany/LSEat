#ifndef _LLISTA_H_
#define _LLISTA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


typedef struct {
	char *name;
	int money;
	int socket;
	int factura;
} Element;

typedef struct Node {
	Element e;
	struct Node *seg;
} Node;

typedef struct {
	Node *pri;
	Node *ant;
} Llista;

Llista LLISTA_crea();

void LLISTA_insereix(Llista *l, Element e);

Element LLISTA_consulta(Llista *l);

void LLISTA_esborra(Llista *l);

void LLISTA_avanca(Llista *l);

void LLISTA_vesInici(Llista *l);

void LLISTA_vesFinal(Llista *l);

int LLISTA_buida(Llista l);

int LLISTA_fi(Llista l);

void LLISTA_destrueix(Llista *l);


#endif
