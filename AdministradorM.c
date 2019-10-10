#include "AdministradorM.h"

/*
*	Crea la lista y le inserta punteros a estructuras Bloque
*	para simular los bloques de memoria de ocho bits c/u.
*/
void iniciarMemoria() {
	tipoAjuste = primerA;
	indiceSegundoAjuste = 0;
	memoriaVirtual = crearLista();
	for (int i = 0; i < MAXBLOQUES; ++i)
	{
		Bloque* bloque = calloc(1, sizeof(Bloque));
		bloque->ocupado = 0;
		insertarFinal(memoriaVirtual, bloque);
	}
	bloquesLibres = MAXBLOQUES;
}

void mostrarMemoria() {
	Bloque* bloque;
	for (int i = 0; i < MAXBLOQUES; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		printf("%d ", bloque->ocupado);
		if (i % 64 == 0 && i != 0) {
			printf("%s\n", " ");
		}
	}
}

int solicitarMemoria(Process* p, int cantidadDeBits) {
	/*
	*	Convierte la cantidad de bits solicitada a bloques
	*	antes de continuar con la asignación.
	*/
	int bloquesIncompletos = (cantidadDeBits % 8 != 0) ? 1 : 0;
	int totalBloques = (cantidadDeBits / 8) + bloquesIncompletos;
	/*
	*	En caso de no hacer memoria suficiente, rechaza la 
	*	solicitud de memoria del proceso.
	*/
	if (totalBloques > bloquesLibres) {
		printf("%s\n", "Espacio insuficiente");
		return 0;
	}

	switch(tipoAjuste) {
		case primerA:
			return primerAjuste(p, totalBloques);
		case segundoA:
			return segundoAjuste(p, totalBloques);
		case mejorA:
			return mejorAjuste(p, totalBloques);
		case peorA:
			return peorAjuste(p, totalBloques);
		case aRapido:
			return ajusteRapido(p, totalBloques);
		default: printf("Algoritmo de administración de memoria no soportado\n");
	}
}

/*
*	El administrador de memoria explora la lista de segmentos 
*	hasta encontrar un hueco que sea lo bastante grande. Después 
*	el hueco se divide en dos partes, una para el proceso y otra 
*	para la memoria sin utilizar, excepto en el estadísticamente 
*	improbable caso de un ajuste exacto. El algoritmo del primer 
*	ajuste es rápido debido a que busca lo menos posible.
*/
int primerAjuste(Process* p, int totalBloques) {
	int pInicio = buscarIndiceInicio(totalBloques);
	int pFinal = 0;
	int ret;
	/*
	*	Si el indice de inicio es distinto de -1 significa que 
	*	encontró un espacio que puede contener la cantidad de
	*	bloques requeridos por el proceso.
	*/
	if (pInicio != -1) {
		pFinal = pInicio + totalBloques;
		ret = marcarOcupado(p, pInicio, pFinal);
		mostrarMemoria();
		return ret;
	} else {
		/*
		*	Si no encuentra un espacio acomoda la lista y lo vuelve a 
		*	intentar conseguirle un espacio, en caso de que denuevo no
		*	se encuentre un espacio se rechaza la solicitud de memoria.
		*/
		acomodarLista();
		pInicio = buscarIndiceInicio(totalBloques);
		if (pInicio == -1) {
			return 0;
		}
		
		pFinal = pInicio + totalBloques;
		ret = marcarOcupado(p, pInicio, pFinal);

		return ret;
	}
	return 0;
}

int segundoAjuste(Process* p, int totalBloques) {
	int pInicio = buscarIndiceInicio(totalBloques);
	int pFinal = 0;
	int ret;
	/*
	*	Si el indice de inicio es distinto de -1 significa que 
	*	encontró un espacio que puede contener la cantidad de
	*	bloques requeridos por el proceso.
	*/
	if (pInicio != -1) {
		pFinal = pInicio + totalBloques;
		ret = marcarOcupado(p, pInicio, pFinal);
		mostrarMemoria();
		return ret;
	} else {
		/*
		*	Si no encuentra un espacio acomoda la lista y lo vuelve a 
		*	intentar conseguirle un espacio, en caso de que denuevo no
		*	se encuentre un espacio se rechaza la solicitud de memoria.
		*/
		acomodarLista();
		pInicio = buscarIndiceInicio(totalBloques);
		if (pInicio == -1) {
			return 0;
		}
		
		pFinal = pInicio + totalBloques;
		ret = marcarOcupado(p, pInicio, pFinal);

		return ret;
	}
	return 0;
}

int mejorAjuste(Process* p, int totalBloques) {

}

int peorAjuste(Process* p, int totalBloques) {

}

int ajusteRapido(Process* p, int totalBloques) {

}

/*Busca el índice del primer bloque en blanco, lo usa el primer ajuste*/
int buscarIndiceInicio(int totalBloques) {
	int indice = -1;
	int encontrados = 0;
	Bloque* bloque;

	/*
	*	Busca un espacio lo suficientemente grande para contener 
	* 	la cantidad de bloques requerida por el proceso.
	*/
	for (int i = 0; i < MAXBLOQUES && encontrados < totalBloques; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		if (!bloque->ocupado) {
			if (indice == -1) {
				indice = i;
			}
			encontrados++;
		} else {
			indice = -1;
			encontrados = 0;
		}
	}
	/*
	*	Valida si el máximo de bloques encontrados concuerda con 
	*	la cantidad de bloques solicitados.
	*/
	if (encontrados >= totalBloques) {
		return indice;
	} else {
		return -1;
	}
}

int buscarIndiceInicioSegundoAjuste(int totalBloques) {
	int indice = -1;
	int encontrados = 0;
	Bloque* bloque;
	int i;
	/*
	*	Busca un espacio lo suficientemente grande para contener 
	* 	la cantidad de bloques requerida por el proceso.
	*/
	for (i = indiceSegundoAjuste; i < MAXBLOQUES && encontrados < totalBloques; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		if (!bloque->ocupado) {
			if (indice == -1) {
				indice = i;
			}
			encontrados++;
		} else {
			indice = -1;
			encontrados = 0;
		}
	}
	/*
	*	Valida si el máximo de bloques encontrados concuerda con 
	*	la cantidad de bloques solicitados.
	*/
	if (encontrados >= totalBloques) {
		indiceSegundoAjuste = indice;
		return indice;
	} else {
		return -1;
	}
}

int marcarOcupado(Process* p, int indiceInicio, int indiceFinal) {
	/*
	*	Valida que el indice del bloque asignado al proceso 
	*	no sobrepase el índice máximo que puede tener.
	*/
	printf("Indice inicio: %d Indice final %d total %d\n", indiceInicio, indiceFinal, (indiceFinal - indiceInicio));
	if (indiceFinal < 1024) {
		Bloque* bloque;
		for (int i = indiceInicio; i < indiceFinal; ++i)
		{
			bloque = obtener(memoriaVirtual, i);
			printf("El bloque %d está %d y pasa a %d\n", i, bloque->ocupado, !bloque->ocupado);
			bloque->ocupado = 1;
			sprintf(bloque->owner, "%d", p->info->id); 
		}
		p->bloqueInicio = indiceInicio;
		p->bloqueFinal = indiceFinal;
		bloquesLibres -= (indiceFinal - indiceInicio);
		printf("%s  %d\n", "Bloques disponibles:", bloquesLibres);
		return 1;
	}
} 

int liberarMemoria(Process* p) {
	Bloque* bloque;
	for (int i = p->bloqueInicio; i < p->bloqueFinal; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		bloque->ocupado = 0;
	}
	p->bloqueInicio = -1;
	p->bloqueFinal = -1;
	return 1;
}

/*
*	Método para escribir bits en los
*	bloques de memoria.
*/
int escribir(Process* p, int* valor) {
	/*
	*	Falta la validación del caso en que no le quede memoria 
	*	al administrador, para guardar las ligaduras.
	*/

	/*
	*	Cantidad de bits a escribir.
	*/
	int cantidadEscritura = sizeof(valor) / sizeof(valor[0]);
	int contador = p->bloqueInicio;
	int i;
	/*
	*	Cantidad de bloques asignados al proceso
	*/
	int cantidad = p->bloqueFinal - p->bloqueInicio;
	Bloque* bloque = obtener(memoriaVirtual, contador);
	
	for (i = 0; i < cantidadEscritura && i < (cantidad * 8); ++i)
	{
		/*
		*	Escribe bit a bit en el espacio del bloque.
		*/
		bloque->espacio[i] = valor[i];
		if (i % 8 == 0 && contador < cantidad) {
			++contador;
			bloque = obtener(memoriaVirtual, contador);
			if (atoi(bloque->owner) != p->info->id) {
				/*
				*	Solo ocho bits para que se le de un único
				*	bloque
				*/
				if (solicitarMemoria(p, 8)) {
					bloque = obtener(memoriaVirtual, contador);
				} else {
					printf("Para el proceso %d no se pudo reservar memoria suficiente para acabar la escritura", p->info->id);
					return 0;
				}
			}
		} 
		
	}
	return 1;
}

/*
*	Usado para guardar memoria para cada 
*	unión de bloques de memoria
*/
int escribirUnionEnMemoria() {
	Bloque *bloque;
ciclo:
	for (int i = 1000; i < MAXBLOQUES; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		if (strcmp(bloque->owner, "AdministradorM") == 0) {
			for (int i = 0; i < 8; ++i)
			{
				if (!bloque->espacio[i]) {
					bloque->espacio[i] = 1;
					return 1;
				}
			}
		}
	}
	if (acomodarLista() > 0) {
		goto ciclo;
	}
	return 0;
}

/*
*	Devuelve la cantidad de espacios libres,
*	-1 en caso de que no hayan	
*/
int acomodarLista() {
	/*
	*	1. Buscar un espacio en blanco.
	*	2. Iterar hasta que se encuentre un nodo ocupado.
	*	3. Copiar el nodo y los demás que estén relacionados.
	*	4. Mover la copia al primer espacio en blanco en adelante.
	*	5. Repetir el algoritmo hasta que no hay nodos para mover.
	*/
	if (bloquesLibres <= 0) {
		goto fin;
	}

	Bloque* bloque;
	int indice = -1;
	int encontrados = 0;
	int espaciosDisponibles = 0;
ciclo:
	for (int i = 0; i < MAXBLOQUES; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		if (!bloque->ocupado) {
			if (indice == -1) {
				indice = i;
			}
			encontrados++;
		} else if (encontrados != 0 && indice != -1) {
			/*
			*	En caso de que se haya encontrado al menos un
			*	cero y a la siguiente iteración hay un uno, se
			*	guarda la cantidad de espacios libres.
			*/
			espaciosDisponibles += encontrados;
			break;
		}
	}
	/*Lista temporal para guardar los bloques que se van a mover*/
	Lista* listaTemp = crearLista();

	/*Guarda los nodos luego del último bloque en blanco*/
	for (int i = (indice + encontrados); i < MAXBLOQUES; ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		if (bloque->ocupado) {
			insertarFinal(listaTemp, bloque);
			bloque->ocupado = 0;
		} else {
			break;
		}
	}

	/*Pega los bloques nuevos*/
	int c = 0;
	for (int i = indice; i < (indice + encontrados); ++i)
	{
		bloque = obtener(memoriaVirtual, i);
		bloque = obtener(listaTemp, c++);
	}
fin:
	printf("No se encontraron bloques libres para acomodar");
	return -1;
}