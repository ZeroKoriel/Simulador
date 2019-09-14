#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H 

#include <stdlib.h>

#include "Nodo.h"

typedef struct Lista 
{
	Nodo* head;
	Nodo* tail;
	int cantidadNodos;
}Lista;

Lista* crearLista();
void destruirLista(Lista*);

void insertar(Lista*, void*);
void* primeroEnLista(Lista*);
void* ultimoEnLista(Lista*);
void* siguiente(Lista*);
void* anterior(Lista*);


#endif