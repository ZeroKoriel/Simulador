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

void crearProceso(int id, int prioridad, TipoProceso tipo, char* file) {
	Process* p;
	p = calloc(1, sizeof(Process));
	p->contadorE = 0;
	p->info = calloc(1, sizeof(processInfo));
	p->info->id = id;
	p->info->prioridad = prioridad;
	p->info->quantum = 100/prioridad;
	p->info->estado = bloqueado;
	p->info->tipo = tipo;
	p->info->contadorPrograma = 0;
	leerArchivo(p->info, file);
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
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) {
		ejecutar(prosessPlanificador->enEjecucion);
		cambioDeContexto(prosessPlanificador->enEjecucion);

		/*Intenta nivelar las listas*/
		int cantidad = prosessPlanificador->cBloqueado->cantidadNodos - prosessPlanificador->cListo->cantidadNodos;
		if (cantidad > 0) {
			for (int i = 0; i < cantidad; ++i)
			{
				Process* temp = obtener(prosessPlanificador->cBloqueado, 0);
				if (temp && temp->info) {
					insertarFinal(prosessPlanificador->cListo, temp);
				}
				eliminarElemento(prosessPlanificador->cBloqueado, 0);
			}
		}
	}
}

void algoritmoRoundRobin() {
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) {
		prosessPlanificador->enEjecucion->info->quantum = 20;
		ejecutar(prosessPlanificador->enEjecucion);
		cambioDeContexto(prosessPlanificador->enEjecucion);
	}
}

void algoritmoSJF() {
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) {
		ejecutar(prosessPlanificador->enEjecucion);
		cambioDeContexto(prosessPlanificador->enEjecucion);
	}
}

void algoritmoPorPrioridad() {
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) {
		ejecutar(prosessPlanificador->enEjecucion);

		++prosessPlanificador->enEjecucion->contadorE;
		if (prosessPlanificador->enEjecucion->contadorE % 2 == 0) {
			if (prosessPlanificador->enEjecucion->info->prioridad > 1) {
				prosessPlanificador->enEjecucion->info->prioridad--;
			} else {
				prosessPlanificador->enEjecucion->info->prioridad = 1 + rand() % 10;
			}
		}
		
		cambioDeContexto(prosessPlanificador->enEjecucion);
	}
}

void algoritmoTR_RMS() {
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) {
		ejecutar(prosessPlanificador->enEjecucion);

		++prosessPlanificador->enEjecucion->contadorE;
		if (prosessPlanificador->enEjecucion->contadorE % 2 == 0) {
			if (prosessPlanificador->enEjecucion->info->prioridad > 1) {
				prosessPlanificador->enEjecucion->info->prioridad--;
			} else {
				prosessPlanificador->enEjecucion->info->prioridad = 1 + rand() % 10;
			}
		}

		cambioDeContexto(prosessPlanificador->enEjecucion);	
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

void ordenarPorTamanio() {
	int size = prosessPlanificador->cListo->cantidadNodos;
	Process *vector[size];

	for (int i = 0; i < size; ++i)
	{
		vector[i] = obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
	}

	destruirLista(prosessPlanificador->cListo);

	for (int step = 0; step < size - 1; ++step)
  	{
    	int swapped = 0;
    	for (int i = 0; i < size - step - 1; ++i)
    	{
      		if (vector[i]->info->cantidadDeLineas > vector[i + 1]->info->cantidadDeLineas)
      		{
        		Process* temp = vector[i];
        		vector[i] = vector[i + 1];
        		vector[i + 1] = temp;
        		swapped = 1;
      		}
    	}
    	if (swapped == 0) {
     		break;
    	}
  	}

  	for (int i = 0; i < size; ++i)
  	{
  		insertarFinal(prosessPlanificador->cListo, vector[i]);
  	}
}

void ordenarPorPrioridad() {
	int size = prosessPlanificador->cListo->cantidadNodos;
	Process *vector[size];

	for (int i = 0; i < size; ++i)
	{
		vector[i] = obtener(prosessPlanificador->cListo, 0);
		eliminarElemento(prosessPlanificador->cListo, 0);
	}

	destruirLista(prosessPlanificador->cListo);

	for (int step = 0; step < size - 1; ++step)
  	{
    	int swapped = 0;
    	for (int i = 0; i < size - step - 1; ++i)
    	{
      		if (vector[i]->info->prioridad < vector[i + 1]->info->prioridad)
      		{
        		Process* temp = vector[i];
        		vector[i] = vector[i + 1];
        		vector[i + 1] = temp;
        		swapped = 1;
      		}
    	}
    	if (swapped == 0) {
     		break;
    	}
  	}

  	for (int i = 0; i < size; ++i)
  	{
  		insertarFinal(prosessPlanificador->cListo, vector[i]);
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
		(50000);
		shedTask();
		escogerAlgoritmo();
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
				if (esPlanificableRMS()){
					algoritmoTR_RMS();
				}
				break;
			case sfj:
				algoritmoSJF();
				break;
			default:
				printf("%s\n", "Opción no soportada");
		}
		usleep(50000);	
	}
}

void escogerAlgoritmo() {
	int cPl = prosessPlanificador->cListo->cantidadNodos;
	int cPb = prosessPlanificador->cBloqueado->cantidadNodos;
	int total = cPl + cPb;
	int cLotes = 0;
	int cInteractivo = 0;
	int cTiempo = 0;
	Process* temp;

	/*El porcentaje se tomaría de las dos colas */

	/*Cuenta los tipos en la lista de listos*/
	for (int i = 0; i < cPl; ++i)
	{
		temp = obtener(prosessPlanificador->cListo, i);
		if (temp && temp->info) {
			switch(temp->info->tipo) {
				case interactivo:
					cInteractivo++;
					break;
				case lotes:
					cLotes++;
					break;
				case tReal:
					cTiempo++;
					break;
			}
		}
	}

	for (int i = 0; i < cPb; ++i)
	{
		temp = obtener(prosessPlanificador->cBloqueado, i);
		if (temp && temp->info) {
			switch(temp->info->tipo) {
				case interactivo:
					cInteractivo++;
					break;
				case lotes:
					cLotes++;
					break;
				case tReal:
					cTiempo++;
					break;
			}
		}
	}

	if (cInteractivo != 0) {
		cInteractivo = total / cInteractivo;
	}

	if (cLotes != 0) {
		cLotes = total / cLotes;
	}

	if (cTiempo != 0) {
		cTiempo = total / cTiempo;
	}

	if (cInteractivo > cLotes && cInteractivo > cTiempo) {
		srand(time(NULL));
		int valor = rand() % 2;
		if (valor) { //Si es 1 escoge SFJ
			prosessPlanificador->algoritmoActual = sfj;
			printf("%s\n", "Cambia a SFJ");
		} else { //Si no escoge round robin
			prosessPlanificador->algoritmoActual = ARoundR;
			printf("%s\n", "Cambia a Round Robin");
		}
	} else if (cLotes > cInteractivo && cLotes > cTiempo) {
		prosessPlanificador->algoritmoActual = AFcfs;
		printf("%s\n", "Cambia a plaficación por lotes");
	} else if (cTiempo > cInteractivo && cTiempo > cLotes) {
		prosessPlanificador->algoritmoActual = tiempoReal;
		printf("%s\n", "Cambia a planificación en tiempo real");
	}
}

/*
* Método con las tareas del planificador, que sería solicitar procesos que pasaran automaticamente 
* a bloqueado y encolar un nuevo proceso a listo.
*/
void shedTask() {
	switch(prosessPlanificador->algoritmoActual) {
		case sfj:
			goto corto;
			break;
		case AFcfs:
			goto normal;
			break;
		case ARoundR:
			goto normal;
			break;
		case APrioridad:
			goto alPrioridad;
			break;
		case tiempoReal:
			goto alPrioridad;
			break;
		default:
			break;
	}

	corto:
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			ordenarPorTamanio();
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}

		if (prosessPlanificador->cListo->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				temp->info->estado = ejecucion;
				prosessPlanificador->enEjecucion = temp;
			}
		} else {
			if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
				Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
				eliminarElemento(prosessPlanificador->cListo, 0);
				if (temp) {
					insertarFinal(prosessPlanificador->cListo, temp);
					ordenarPorTamanio();
				} 
			}
		}
	return;

	normal:
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}
		if (prosessPlanificador->cListo->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				temp->info->estado = ejecucion;
				prosessPlanificador->enEjecucion = temp;
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
	return;

	alPrioridad:
		if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
			insertarFinal(prosessPlanificador->cListo, (Process*)obtener(prosessPlanificador->cBloqueado, 0));
			ordenarPorPrioridad();
			eliminarElemento(prosessPlanificador->cBloqueado, 0);
		}

		if (prosessPlanificador->cListo->cantidadNodos > 0) {
			Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
			eliminarElemento(prosessPlanificador->cListo, 0);
			if (temp) {
				temp->info->estado = ejecucion;
				prosessPlanificador->enEjecucion = temp;
			}
		} else {
			if (prosessPlanificador->cBloqueado->cantidadNodos > 0) {
				Process* temp = (Process*) obtener(prosessPlanificador->cListo, 0);
				eliminarElemento(prosessPlanificador->cListo, 0);
				if (temp) {
					insertarFinal(prosessPlanificador->cListo, temp);
					ordenarPorPrioridad();
				} 
			}
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
		if (info->tipo == lotes) {
			srand(time(NULL));
			int val = rand() % 10;
			if (val != 9) {
				info->estado = bloqueado;
				pthread_cond_signal(&cPlanificador);
				pthread_cond_wait(&(info->cond), &(info->mutex));
			}
		}

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
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 

	cairo_set_font_size(cr, 30);             
    
    cairo_move_to(cr, 330, 75);              
    cairo_show_text(cr, "Ejecución"); 		  

	cairo_move_to(cr, 20, 250);
	cairo_show_text(cr, "Bloqueado");

	cairo_move_to(cr, 650, 250);
	cairo_show_text(cr, "Listo");
}

void drawProcess(cairo_t* cr) {
	int i; 
	int pos;
	char buffer[100];
	cairo_set_font_size(cr, 15);
	cairo_set_source_rgb(cr, 0.1, 0.95, 0.85);
	
	/*Muestra al proceso en ejecución*/
	if (prosessPlanificador->enEjecucion && prosessPlanificador->enEjecucion->info) { 
		cairo_move_to(cr, 350, 100);
		sprintf(buffer,"Proceso %d", prosessPlanificador->enEjecucion->info->id);
		cairo_show_text(cr, buffer);
	}

	pos = 275;
	int cantidadL = prosessPlanificador->cListo->cantidadNodos;

	for (int i = 0; i < cantidadL; ++i)
	{
		Process* temp = obtener(prosessPlanificador->cListo, i);
		if (temp && temp->info) {
			sprintf(buffer, "Proceso %d P: %d", temp->info->id, temp->info->prioridad);
			cairo_move_to(cr, 650, pos);
			cairo_show_text(cr, buffer);
			pos += 20;
		}
	}

	pos = 275;
	int cantidadB = prosessPlanificador->cBloqueado->cantidadNodos;

	for (int i = 0; i < cantidadB; ++i)
	{
		Process* temp = obtener(prosessPlanificador->cBloqueado, i);
		if (temp && temp->info) {
			sprintf(buffer, "Proceso %d P: %d", temp->info->id, temp->info->prioridad);
			cairo_move_to(cr, 20, pos);
			cairo_show_text(cr, buffer);
			pos += 20;
		}
	}

	cairo_move_to(cr, 200, 250);
	sprintf(buffer, "%d", prosessPlanificador->cBloqueado->cantidadNodos);
	cairo_show_text(cr, buffer);

	cairo_move_to(cr, 750, 250);
	sprintf(buffer, "%d", prosessPlanificador->cListo->cantidadNodos);
	cairo_show_text(cr, buffer);
} 