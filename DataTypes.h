#ifndef DATATYPES_H
#define DATATYPES_H 

#include <string.h>
#include <pthread.h>
#include "ADT/Lista.h"

typedef enum Algoritmo
{
	AFcfs = 0,
	ARoundR,
	APrioridad,
	tiempoReal	
}Algoritmo;

typedef enum palabraEstado
{
	bloqueado = 0,
	listo,
	ejecucion, 
	terminado
}palabraEstado;

typedef struct processInfo
{
	int id;
	int prioridad;
	int quantum;
	int contadorPrograma;
	int cantidadDeLineas;

	int TRISA[32];
	int PORTB[32];
	int STATUS[32];
	int PORTA[32];

	char** archivoInstrucciones;

	palabraEstado estado;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
}processInfo;

typedef struct Process
{
	pthread_t hilo;	
	processInfo* info;
}Process;

typedef struct shedInfo
{
	/*Colas para el planificador de procesos*/
	Lista* cListo;
	Lista* cBloqueado;
	Process* enEjecucion;
	processInfo* info;

	Algoritmo algoritmoActual;
	int cantidadDeProcesos;
	pthread_t hilo;
}shedInfo;

#endif