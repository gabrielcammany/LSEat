#include "../include/llista.h"

Llista LLISTA_crea() {
	Llista l;
	l.pri = (Node *) malloc(sizeof(Node));

	if (l.pri != NULL) {
		l.pri->seg = NULL;
		l.ant = l.pri;
	}

	return l;
}


void LLISTA_insereix(Llista *l, Element e) {
	Node *aux;
	aux = (Node *) malloc(sizeof(Node));
	if (aux == NULL) {
		write(1, "Error!, no s'ha pogut crear el node aux\n", strlen("Error!, no s'ha pogut crear el node aux\n"));
	} else {
		aux->e = e;
		aux->seg = l->ant->seg;
		l->ant->seg = aux;
		l->ant = l->ant->seg;
	}

}

Element LLISTA_consulta(Llista *l) {
	Element e;
	if (l->ant->seg == NULL) {
		e.money = -1;
		return e;
	} else {
		return l->ant->seg->e;
	}
}

void LLISTA_esborra(Llista *l) {
	if (l->ant->seg == NULL) {
		write(1, "Error! No es pot esborrar el següent element\n",
			  strlen("Error! No es pot esborrar el següent element\n"));
	} else {
		Node *aux;
		aux = l->ant->seg;
		l->ant->seg = l->ant->seg->seg;
		close(aux->e.socket);
		aux->seg = NULL;
		free(aux);
	}
}

void LLISTA_avanca(Llista *l) {
	if (l->ant->seg == NULL) {
		write(1, "Error! No puc avancar!\n", strlen("Error! No puc avancar!\n"));
	} else {
		l->ant = l->ant->seg;
	}
}

void LLISTA_vesInici(Llista *l) {
	l->ant = l->pri;
}

void LLISTA_vesFinal(Llista *l) {
	while (l->ant->seg != NULL) {
		l->ant = l->ant->seg;
	}
}

int LLISTA_buida(Llista l) {
	if (l.pri->seg == NULL) {
		return 1;
	} else {
		return 0;
	}
}

int LLISTA_fi(Llista l) {
	if (l.ant->seg == NULL) {
		return 1;
	} else {
		return 0;
	}
}

void LLISTA_destrueix(Llista *l) {
	l->ant = l->pri;
	while (l->pri->seg != NULL) {
		LLISTA_esborra(l);
	}
	free(l->ant);
}

