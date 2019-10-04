#include "AdministradorM.h"

/*
*	Crea la lista y le inserta punteros a estructuras Bloque
*	para simular los bloques de memoria de ocho bits c/u.
*/
void iniciarMemoria() {
	memoriaVirtual = crearLista();
	for (int i = 0; i < MAXBLOQUES; ++i)
	{
		Bloque* bloque = calloc(1, sizeof(Bloque));
		bloque->ocupado = 0;
		insertarFinal(memoriaVirtual, bloque);
	}
	bloquesLibres = MAXBLOQUES;
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

	int pInicio = buscarIndiceInicio(totalBloques);
	int pFinal = 0;

	/*
	*	Si el indice de inicio es distinto de -1 significa que 
	*	encontró un espacio que puede contener la cantidad de
	*	bloques requeridos por el proceso.
	*/
	if (pInicio != -1) {
		pFinal = pInicio + totalBloques;
		return marcarOcupado(p, pInicio, pFinal);
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
		return marcarOcupado(p, pInicio, pFinal);
	}
	return 0;
}

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
	if (encontrados == totalBloques) {
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
	if (indiceFinal < 1024) {
		Bloque* bloque;
		for (int i = indiceInicio; i < indiceFinal; ++i)
		{
			bloque = obtener(memoriaVirtual, i);
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

void acomodarLista() {

}