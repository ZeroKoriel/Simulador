#ifndef PROCESO_H
#define PROCESO_H 

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <glib/gmacros.h>
#include <cairo.h>

#include "ADT/Lista.h"

/*
*     Variables de la estructura processInfo
*
* - Contador de programa	
* - Apuntador a pila		Falta
* - Asignación de memoria	Falta
* - Archivos abiertos		Falta
* - Palabra de estado
* - Id de proceso
* - Registros				Falta
* - Prioridad
*/

#define CANTIDADHILOS 5
#define IDPLANIFICADOR 1000
#define MAXL 10000

#include "DataTypes.h"
#include "ArchivoBusiness.h"

static pthread_cond_t cPlanificador;
static pthread_mutex_t mPlanificador;

/*estructura con la info necesaria para el planificador*/
shedInfo *prosessPlanificador;
Process *process[CANTIDADHILOS];

char* procesoEnEjecucion;
char** listaListos;
char** listaBloqueados;

void initPlanificador();
void initHilos();
void ejecutar(Process*);
void cambioDeContexto(Process*);

void algoritmoFCFS();
void algoritmoRoundRobin();
void algoritmoPorPrioridad();
void algoritmoTR_RMS();

bool esPlanificableRMS();
void ordenarPorPrioridad();
void shedTask();

void eliminarProceso(Process*);
void balancearColas();
void* runPlanificador(void*);
void* runProceso(void*);
bool revisarInterrupciones();
void drawCicle(cairo_t*);
void drawProcess(cairo_t*); 
void mostrarInformacion();

/*
	Para calcular la prioridad en el algoritmo de planificación
	en tiempo real de tipo RMS lo que se hace es dividir 
	1000/quantum	
*/

#endif