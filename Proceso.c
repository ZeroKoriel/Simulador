#include "Proceso.h"


void initPlanificador(){
	/*Inicializa las variables para mostrar la información de los hilos*/
	procesoEnEjecucion = calloc(1, sizeof(char));
    listaListos = calloc(10, sizeof(char*));
    listaBloqueados = calloc(10, sizeof(char*));

	/*Inicializa la estructura del planificador*/
	prosessPlanificador = calloc(1, sizeof(shedInfo));
	prosessPlanificador->cListo = crearLista();
	prosessPlanificador->cBloqueado = crearLista();
	prosessPlanificador->enEjecucion = calloc(1, sizeof(Process));

	prosessPlanificador->info = calloc(1, sizeof(processInfo));
	prosessPlanificador->info->estado = ejecucion;
	prosessPlanificador->cantidadDeProcesos = 0;
	prosessPlanificador->algoritmoActual = ARoundR;

	prosessPlanificador->pPlanificador = calloc(1, sizeof(Process));
	prosessPlanificador->pPlanificador->info = calloc(1, sizeof(processInfo));
	prosessPlanificador->pPlanificador->info->id = -1;
	prosessPlanificador->pPlanificador->info->prioridad = 10;
	prosessPlanificador->pPlanificador->info->quantum = 100/10;
	prosessPlanificador->pPlanificador->info->estado = bloqueado;
	prosessPlanificador->pPlanificador->info->contadorPrograma = 0;

	pthread_create(&(prosessPlanificador->hilo), NULL, (void*)runPlanificador, (void*)(prosessPlanificador));
}

void crearProceso(int id, int prioridad, TipoProceso tipo) {
	Process* p;
	p = calloc(1, sizeof(Process));
	p->info = calloc(1, sizeof(processInfo));
	p->info->id = id;
	p->info->prioridad = prioridad;
	p->info->quantum = 100/prioridad;
	p->info->estado = bloqueado;
	p->info->tipo = tipo;
	p->info->contadorPrograma = 0;
	leerArchivo(p->info);
	pthread_create(&(p->hilo), NULL, (void*)runProceso, (void*)(p->info));
	insertarFinal(prosessPlanificador->cBloqueado, (void*)p);
}

/*Guarda al proceso en ejecución y le da la señal para que inicie*/
void ejecutar(Process* proceso) {
	pthread_mutex_lock(&mPlanificador);

	prosessPlanificador->enEjecucion = proceso;
	pthread_cond_signal(&(proceso->info->cond));

	pthread_cond_wait(&cPlanificador, &mPlanificador);
	pthread_mutex_unlock(&mPlanificador);
}

/*Asigna un proceso a una cola dependiendo de su estado*/
void cambioDeContexto(Process* proceso) {
		if (proceso) {
		switch (proceso->info->estado) {
			case bloqueado:
				insertarFinal(prosessPlanificador->cBloqueado, (void*)proceso);
				break;
			case listo:
				insertarFinal(prosessPlanificador->cListo, (void*)proceso);
				break;
			case terminado:
				eliminarProceso(proceso);
				break;
		}
	}
}

/*Libera la memoria reservada para la estructura del proceso*/
void eliminarProceso(Process* proceso) {
	if (proceso->info) {
		free(proceso->info);
		proceso->info = NULL;
	}
	free(proceso);
	proceso = NULL;
}

/*Algoritmo de planificacion usado por el hilo planificador*/
void algoritmoFCFS() {
	if (prosessPlanificador->cListo->cantidadNodos > 0) {
		Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
		if (temp) {
			temp->info->estado = ejecucion;
			ejecutar(temp);
			cambioDeContexto(temp);
		}
	} else {
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				insertarFinal(prosessPlanificador->cListo, temp);
			} 
		}
	}
}

void algoritmoRoundRobin() {
	if (prosessPlanificador->cListo->cantidadNodos > 0) {
		Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
		if (temp) {
			temp->info->estado = ejecucion;
			temp->info->quantum = 20;
			ejecutar(temp);
			cambioDeContexto(temp);
		}
	} else {
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cBloqueado, 0);
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
			if (temp) {
				insertarFinal(prosessPlanificador->cListo, temp);
			} 
		}
	}
}

void algoritmoPorPrioridad() {
	if (prosessPlanificador->cListo->cantidadNodos > 0) {
		Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
		if (temp) {
			temp->info->estado = ejecucion;
			ejecutar(temp);
			cambioDeContexto(temp);
		}
	} else {
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				insertarFinal(prosessPlanificador->cListo, temp);
			} 
		}
	}
}

void algoritmoTR_RMS() {
	if (prosessPlanificador->cListo->cantidadNodos > 0 && esPlanificableRMS()) {
		Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
		if (temp) {
			temp->info->estado = ejecucion;
			ejecutar(temp);
			cambioDeContexto(temp);
		}
	} else {
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				insertarFinal(prosessPlanificador->cListo, temp);
			} 
		}
	}	
}

bool esPlanificableRMS() {
	int i;
	float suma = 0;
	Process* temp;

	for (i = 0; i < prosessPlanificador->cListo->cantidadNodos; ++i)
	{
		temp = (Process*) obtener(prosessPlanificador->cListo, i);
		suma += (temp->info->quantum /100);
	}
	return suma < 1;
}

void ordenarPorPrioridad() {
	Process *respaldo[prosessPlanificador->cListo->cantidadNodos];
	Process *temp;
	int cantidad = prosessPlanificador->cListo->cantidadNodos;
	int i;
	int j;
	for (i = 0; i < cantidad; ++i)
	{
		respaldo[i] = obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
	}
	
	destruirLista(prosessPlanificador->cListo);

	for (i = 0; i < cantidad; ++i)
	{
		for (j = 0; j < cantidad; ++j)
		{
			if(respaldo[i]->info->prioridad > respaldo[j]->info->prioridad) {
				temp = respaldo[i];
				respaldo[i] = respaldo[j];
				respaldo[j] = temp;
			}
		}
	}
	for (i = 0; i < cantidad; ++i)
	{
		insertarFinal(prosessPlanificador->cListo, (void*)respaldo[i]);
	}
}

/*
* Método para el caso de que hayan muchos procesos en bloqueado y pocos en listo
* hace que ambas tengan el mismo tamaño. 
*
* No funciona si hay muchos procesos en listo.
*/
void balancearColas() {
	int val;
	Process* temp;
	if (prosessPlanificador->cBloqueado->cantidadNodos > prosessPlanificador->cListo->cantidadNodos) {  /*Caso en que cBloqueado tenga más procesos que cListo*/
		val = (prosessPlanificador->cBloqueado->cantidadNodos + prosessPlanificador->cListo->cantidadNodos) / 2;
		val = prosessPlanificador->cBloqueado->cantidadNodos - val;
		for (int i = 0; i < val; ++i)
		{
			temp =(Process*) obtener(prosessPlanificador->cBloqueado, 0);
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
			if (temp) {
				temp->info->estado = listo;
				insertarFinal(prosessPlanificador->cListo, (void*)temp);
			}
		}
	}
}

/*Método que ejecuta el hilo planificador*/
void* runPlanificador(void* args) {
	while (true) {
		usleep(100000);
		shedTask();
		switch(prosessPlanificador->algoritmoActual) {
			case AFcfs:
				algoritmoFCFS();
				break;
			case ARoundR:
				algoritmoRoundRobin();
				break;
			case APrioridad:
				algoritmoPorPrioridad();
				break;
			case tiempoReal:
				algoritmoTR_RMS();
				break;
			default:
				printf("%s\n", "Opción no soportada");
		}
		usleep(100000);	
		mostrarInformacion();
	}
}

/*
* Método con las tareas del planificador, que sería solicitar procesos que pasaran automaticamente 
* a bloqueado y encolar un nuevo proceso a listo.
*/
void shedTask() {
	switch(prosessPlanificador->algoritmoActual) {
		case AFcfs:
			goto fcfs;
			break;
		case ARoundR:
			goto robin;
			break;
		case APrioridad:
			goto alPrioridad;
			break;
		case tiempoReal:
			goto alPrioridad;
		default:
			break;
	}
	fcfs:
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}
	return;

	robin:
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}
	return;

	alPrioridad:
		ordenarPorPrioridad();
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}
	return;
}

/*
* Método que ejecutan los hilos proceso, con el bloqueo del planificador. Haciendo 
* que espera una señal.
*/
void* runProceso(void* args) {
	processInfo* info = (processInfo*) args;
	pthread_mutex_lock(&(info->mutex));
	pthread_cond_wait(&(info->cond), &(info->mutex));
	
	while (info->contadorPrograma < info->cantidadDeLineas) {
		ejecutarInstruccion(info);
		
		if (revisarInterrupciones()) {
			info->estado = bloqueado;
			pthread_cond_signal(&cPlanificador);
			pthread_cond_wait(&(info->cond), &(info->mutex));
		}
		
		if (prosessPlanificador->algoritmoActual != AFcfs) {
			--info->quantum;
			if (info->quantum == 0) {
				info->estado = listo;
				pthread_cond_signal(&cPlanificador);
				pthread_cond_wait(&(info->cond), &(info->mutex));
			}
		}
		usleep(100);
	}
	pthread_mutex_unlock(&(info->mutex));
	pthread_cond_signal(&cPlanificador);
	info->estado = terminado;
}

/*En proceso, preguntar*/
bool revisarInterrupciones() {
	srand(time(NULL));
	int valor= rand() % 1000;
	return valor < 500;
}

/*Dibuja las palabras del cicho*/
void drawCicle(cairo_t* cr) {
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);  /*asigna el color para pintar*/

	cairo_set_font_size(cr, 30);              /*asigna el tamaño para dibujar*/
    
    cairo_move_to(cr, 330, 75);              /*mueve las coordenadas antes de dibujar*/
    cairo_show_text(cr, "Ejecución"); 		  /*dibuja texto*/

	cairo_move_to(cr, 20, 250);
	cairo_show_text(cr, "Bloqueado");

	cairo_move_to(cr, 650, 250);
	cairo_show_text(cr, "Listo");
}

void drawProcess(cairo_t* cr) {
	int i; 
	int pos;
	char buffer[20];
	cairo_set_font_size(cr, 15);
	cairo_set_source_rgb(cr, 0.1, 0.95, 0.85);
	
	/*Muestra al proceso en ejecución*/
	cairo_move_to(cr, 350, 100);
	cairo_show_text(cr, procesoEnEjecucion);
	//procesoEnEjecucion = " ";
	/*Muestra los procesos en bloqueado*/
	pos = 275;
	for (i = 0; i < 10; ++i)
	{
		cairo_move_to(cr, 20, pos);
	    cairo_show_text(cr, listaBloqueados[i]);
		pos += 20;
	}

	/*Muestra los procesos en listo*/
	pos = 275;
	for (i = 0; i < 10; ++i)
	{
		cairo_move_to(cr, 650, pos);
	    cairo_show_text(cr, listaListos[i]);
		pos += 20;
	}

	for (int i = 0; i < 10; ++i)
	{
		listaBloqueados[i] = " ";
		listaListos[i] = " ";
	}

	cairo_set_source_rgb(cr,0.2,1,0);
	/*Cantidad de bloqueados*/
	sprintf(buffer,"Cantidad %d", prosessPlanificador->cBloqueado->cantidadNodos);
	
	cairo_move_to(cr, 20, pos);
	cairo_show_text(cr, buffer);

	/*Cantidad de listos*/
	sprintf(buffer,"Cantidad %d", prosessPlanificador->cListo->cantidadNodos);
	
	cairo_move_to(cr, 670, pos);
	cairo_show_text(cr, buffer);
} 

void mostrarInformacion() {
	int i; 
	char p[20];
	Process* temp;
	int cantidadListo = prosessPlanificador->cListo->cantidadNodos;
	int cantidadBloqueado = prosessPlanificador->cBloqueado->cantidadNodos;
	if (prosessPlanificador->enEjecucion != NULL && prosessPlanificador->enEjecucion->info) {
		assert(prosessPlanificador->enEjecucion != NULL);
		sprintf(p, "Proceso %d", prosessPlanificador->enEjecucion->info->id);
		procesoEnEjecucion = strdup(p);
	}
	for (i = 0; i < cantidadListo; ++i)
	{
		temp = (Process*) obtener(prosessPlanificador->cListo, i);
		if (temp) {
			if (i < 10) {	
			sprintf(p,"Proceso %d p: %d", temp->info->id, temp->info->prioridad);
			listaListos[i] = strdup(p);
			}
		}
	}

	for (i = 0; i < cantidadBloqueado; ++i)
	{
		temp = (Process*) obtener(prosessPlanificador->cBloqueado, i);
		if (temp) {
			if (i < 10) {
			sprintf(p,"Proceso %d p: %d", temp->info->id, temp->info->prioridad);	
			listaBloqueados[i] = strdup(p);
			}
		}
	}
	/**/
}