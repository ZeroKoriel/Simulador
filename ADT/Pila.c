#include "Pila.h"

Pila* crearPila(){
	Pila* pila = calloc(1, sizeof(Pila));
	pila->top = NULL;
	pila->cantidadDeNodos = 0;
	return pila;
}

void destruirPila(Pila* pila) {
	while (pila->top) {
		Nodo* eliminado = pila->top;
		pila->top = pila->top->siguiente;
		destruirNodo(eliminado);
	}
	free(pila);
}

void push(Pila* pila, void* data) {
	Nodo* nodo = crearNodo(data);
	nodo->siguiente = pila->top;
	pila->top = nodo;
	++pila->cantidadDeNodos;
}

void* pop(Pila* pila) {
	if (!pila->top) {
		return NULL;
	}
	Nodo* eliminado = pila->top;
	void* data = eliminado->data;
	pila->top = pila->top->siguiente;
	destruirNodo(eliminado);
	--pila->cantidadDeNodos;
	return data;
}

void* top(Pila* pila) {
	if (!pila->top) {
		return NULL;
	}
	return pila->top->data;
}