#include "Cola.h"

Cola* crearCola() {
	Cola* cola = calloc(1, sizeof(Cola));
	cola->primero = NULL;
	cola->ultimo = NULL;
	cola->cantidadDeNodos = 0;
	return cola;
}

void destruirCola(Cola* cola) {
	while (cola->primero) {
		Nodo* eliminado = cola->primero;
		cola->primero = cola->primero->siguiente;
		destruirNodo(eliminado);	
	}
}

void* front(Cola* cola) {
	if (cola->primero) {
		return cola->primero->data;
	} 
	return NULL;
}

void enqueue(Cola* cola, void* data) {
    Nodo* temp = crearNodo(data);
    if (!cola->primero) {
    	cola->primero = temp;
    	cola->ultimo = temp;
    	++cola->cantidadDeNodos;
    } else {
    	cola->ultimo->siguiente = temp;
    	cola->ultimo = temp;
    	++cola->cantidadDeNodos;
    }
}

void* dequeue(Cola* cola) { 
    if (cola->primero) {
    	Nodo* eliminado = cola->primero;
    	void* data = eliminado->data;
    	cola->primero = cola->primero->siguiente;
    	destruirNodo(eliminado);

    	if (!cola->primero) {
    		cola->ultimo = NULL;
    	}
        --cola->cantidadDeNodos;
    	return data;
    } 
}
