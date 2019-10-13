#ifndef ADMINISTRADORM_H
#define ADMINISTRADORM_H 

#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"

#define MAXBLOQUES 1024

typedef enum Ajuste
{
	primerA = 0,
	segundoA,
	mejorA,
	peorA,
	aRapido
}Ajuste;

typedef struct EspacioBloque
{
	int inicio;
	int final;
	int cantidad;
}EspacioBloque;

typedef struct TablaIndice
{
	int cantidadB;
	int *indices;
}TablaIndice;

static int bloquesLibres;
static int indiceSegundoAjuste;

TablaIndice *indices[MAXBLOQUES];
Lista *listaEspacios;
Ajuste tipoAjuste;


int primerAjuste(Process*, int);
int segundoAjuste(Process*, int);
int mejorAjuste(Process*, int);
int peorAjuste(Process*, int);
int ajusteRapido(Process*, int);

void iniciarMemoria();

int solicitarMemoria(Process*, int);
int liberarMemoria(Process*);
int escribir(Process*, int*);

/*
*	Usado para guardar memoria para cada 
*	unión de bloques de memoria
*/
int escribirUnionEnMemoria();

int buscarIndiceInicio(int);
int buscarIndiceInicioSegundoAjuste(int);
int buscarIndiceMejorAjuste(int);
int buscarIndicePeorAjuste(int);
int marcarOcupado(Process*, int, int);
int acomodarLista();

void insertarAlIndice(int);
int busarIndicesPorTamanio(Process*, int, int*);

void mostrarMemoria();

#endif