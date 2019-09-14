#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "Nodo.h"

typedef struct Cola
{
	Nodo* primero;
	Nodo* ultimo;
	int cantidadDeNodos;
}Cola;

Cola* crearCola();
void destruirCola(Cola*);
void* front(Cola*);
void enqueue(Cola*, void*);
void* dequeue(Cola*);
#endif