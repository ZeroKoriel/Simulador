#ifndef DATATYPES_H
#define DATATYPES_H 

#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "ADT/Lista.h"

#define BITS 32

typedef enum Tipo
{
	interactivo = 0,
	lotes,
	tReal
}TipoProceso;

typedef enum Algoritmo
{
	AFcfs = 0,
	ARoundR,
	APrioridad,
	tiempoReal, 
	sfj	
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
	TipoProceso tipo;

	int PORTA[BITS];
	int PORTB[BITS];

	int TRISA[BITS];
	int TRISB[BITS];
	
	int STATUS[BITS];
	
	int f[BITS];
	int w[BITS];
	
	char** archivoInstrucciones;

	palabraEstado estado;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
}processInfo;

typedef struct Process
{
	pthread_t hilo;	
	processInfo* info;
	int contadorE;
}Process;

typedef struct shedInfo
{
	/*Colas para el planificador de procesos*/
	Lista* cListo;
	Lista* cBloqueado;
	Process* enEjecucion;
	Process* pPlanificador;
	processInfo* info;

	Algoritmo algoritmoActual;
	int cantidadDeProcesos;
	pthread_t hilo;
}shedInfo;

#endif