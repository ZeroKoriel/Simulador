#ifndef PILA_H
#define PILA_H 

#include <stdlib.h>
#include "Nodo.h"

typedef struct Pila
{
	Nodo* top;
	int cantidadDeNodos;
}Pila;

Pila* crearPila();
void destruirPila(Pila*);
void push(Pila*, void*);
void* pop(Pila*);
void* top(Pila*);

#endif