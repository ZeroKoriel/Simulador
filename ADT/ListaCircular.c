#include "ListaCircular.h"

Lista* crearLista() {
	Lista* lista = calloc(1, sizeof(Lista));
	lista->head = NULL;
	lista->tail = NULL;
	lista->cantidadNodos = 0;
	return lista;
}

void destruirLista(Lista* lista) {
	while (lista->head) {
		Nodo* eliminado = lista->head;
		lista->head = lista->head->siguiente;
		destruirNodo(eliminado);
	}
}

void insertar(Lista* lista, void* data) {
	Nodo* tmpI = lista->head;
	Nodo* nuevo = crearNodo(data);

	if (!tmpI) {
		lista->head = lista->tail = nuevo;
	} else {
		while (tmpI != lista->tail) {
			tmpI = tmpI->siguiente;
		}
		tmpI->siguiente = nuevo;
		nuevo->anterior = tmpI;
		lista->tail = nuevo;
	}
	lista->tail->siguiente = lista->head;
	lista->head = lista->tail;
	++lista->cantidadNodos;
}

void* siguiente(Lista* lista) {
	if (!lista->head) {
		return NULL;
	}
	lista->head = lista->head->siguiente;
	return lista->head->data;
}

void* anterior(Lista* lista) {
	if (!lista->head) {
		return NULL;
	}
	Nodo* temp = lista->head;
	lista->head = temp->anterior;
	lista->head->siguiente = temp->siguiente;
	return lista->head->data;
}

void* primeroEnLista(Lista* lista) {
	if (!lista->head) {
		return NULL;
	}
	return lista->head->data;
}

void* ultimoEnLista(Lista* lista) {
	if (!lista->head) {
		return NULL;
	}
	return lista->tail->data;
}