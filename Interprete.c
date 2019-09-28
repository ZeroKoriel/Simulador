#include "Interprete.h"

void ejecutarInstruccion(processInfo* p) {
	if (p->contadorPrograma >= p->cantidadDeLineas && p->contadorPrograma >= 0) {
		return; /*Regresa porque se alcanzó el final del archivo*/
	} 
	
	char* line = p->archivoInstrucciones[p->contadorPrograma];
	buscarLinea(p, line);
}

void buscarLinea(processInfo* p, char* line) {
	char* nemoTemp = calloc(1, sizeof(char));
	char* valorTemp = calloc(1, sizeof(char));

	nemoTemp = strtok(line, " ");
	valorTemp = strtok(NULL, " ");
	
	if (nemoTemp) {
		comprobarNemotecnico(p, nemoTemp, valorTemp);
	} else {
		char* lineTemp = strtok(p->archivoInstrucciones[p->contadorPrograma], " ");
		while (strcmp(lineTemp, " ") == 0) {
			++p->contadorPrograma;
			lineTemp = strtok(p->archivoInstrucciones[p->contadorPrograma], " ");
		}
		valorTemp = strtok(NULL, " ");
		comprobarNemotecnico(p, lineTemp, valorTemp);
	}
}

void comprobarNemotecnico(processInfo* p, char* nemo, char* valor) {
	if (strcmp(nemo, "CLRF") == 0) {          
		mCLRF(p, valor);
	} else if (strcmp(nemo, "CLRW") == 0) {         
		mCLRW(p);
	} else if (strcmp(nemo, "MOVWF") == 0) {         
		mMOVWF(p, valor);
	} else if (strcmp(nemo, "ADDLW") == 0) { 
		mADDLW(p, valor);
	} else if (strcmp(nemo, "ANDLW") == 0) {              
		mANDLW(p, valor);
	} else if (strcmp(nemo, "GOTO") == 0) {               
		mGOTO(p, valor);
	} else if (strcmp(nemo, "IORLW") == 0) {              
		mIORLW(p, valor);
	} else if (strcmp(nemo, "SUBLW") == 0) { 
		mSUBLW(p, valor);
	} else if (strcmp(nemo, "ADDWF") == 0) { /*suma w con f  se guarda en w*/
		mADDWF(p, valor);
	} else if (strcmp(nemo, "ANDWF") == 0) {  
		mANDWF(p, valor);
	} else if (strcmp(nemo, "MOVF") == 0) {  
		mMOVF(p, valor);
	} else if (strcmp(nemo, "SUBWF") == 0) { //SUBWF f,d ejecuta f-w y guarda en d
		mSUBWF(p, valor);
	} else if (strcmp(nemo, "BCF") == 0) {
		mBCF(p, valor);
	} else if (strcmp(nemo, "BSF") == 0) {      
		mBSF(p, valor);
	} else if (strcmp(nemo, "CALL") == 0) {        
		mCALL(p, valor);
	} else if (strcmp(nemo, "RETURN") == 0) {      
		mRETURN(p, valor);
	} else if (strcmp(nemo, "END") == 0) {         
		p->contadorPrograma = p->cantidadDeLineas;
	} else if (strcmp(nemo, "BTFSC") == 0) {      
		mBTFSC(p, valor);
	} else if (strcmp(nemo, "MOVLW") == 0) {        
		mMOVLW(p, valor);
	} 
}

void mCLRF(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTB") == 0) {
		limpiarRegistro(p->PORTB);
	} else if (strcmp(valor, "PORTA") == 0) {
		limpiarRegistro(p->PORTA);
	} else if (strcmp(valor, "STATUS") == 0) {
		limpiarRegistro(p->STATUS);
	} else if (strcmp(valor, "TRISA") == 0) {
		limpiarRegistro(p->TRISA);
	} else if (strcmp(valor, "TRISB") == 0) {
		limpiarRegistro(p->TRISB);
	}
	++p->contadorPrograma;
}

void mCLRW(processInfo* p) {
	limpiarRegistro(p->w);
	++p->contadorPrograma;
}

void mMOVWF(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTA") == 0) {
		copiarRegistro(p->PORTA, p->w);
	} else if (strcmp(valor, "PORTB") == 0) {
		copiarRegistro(p->PORTB, p->w);
	} else if (strcmp(valor, "STATUS") == 0) {
		copiarRegistro(p->STATUS, p->w);
	} else if (strcmp(valor, "TRISA") == 0) {
		copiarRegistro(p->TRISA, p->w);
	} else if (strcmp(valor, "TRISB") == 0) {
		copiarRegistro(p->TRISB, p->w);
	}
	++p->contadorPrograma;
}

void mADDLW(processInfo* p, char* valor) { 
	int i;
	int c = 0;
	int temp[32];
	char* vTemp = strdup(valor);
	int size = strlen(vTemp);
	int contador = size -1;
	
	/*Convierte el vector pasado por parámetro a uno de enteros*/
	for (i = 31; i >= 0; --i)
	{
		if (contador >= 0) {
			temp[i] = vTemp[i] - '0';
		} else {
			temp[i] = 0;
		}
		contador--;
	}
	/*Suma los vectores*/
	for (i = 31; i >= 0; --i)
	{
		if (p->w[i] != temp[i]){
	    	if (c == 1) {
	    		p->w[i] = 0;
	    		c= 1;
	       	} else if (c == 0) {
	       		p->w[i] = 1;
	       		c = 0;  
	       	}
	    }else{
	    	if (p->w[i] == 0) {
	       		if (c == 1) {
	       			p->w[i] = 1;
	       			c = 0;   
	       		} else {
	       			p->w[i] = 0;
	      			c = 0;
	           	}
	       	}else {
	       		if (c ==1) {
	       			p->w[i] = 1;
	       			c = 1;
	      		} else {
	       			p->w[i] = 0;
	       			c = 1;
	       		}
	    	}	
	    }
	}
	++p->contadorPrograma;
}

void mANDLW(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTA") == 0) {
		andRegistros(p->PORTA, p->w);
	} else if (strcmp(valor, "PORTB") == 0) {
		andRegistros(p->PORTB, p->w);
	} else if (strcmp(valor, "STATUS") == 0) {
		andRegistros(p->STATUS, p->w);
	} else if (strcmp(valor, "TRISA") == 0) {
		andRegistros(p->TRISA, p->w);
	}
	++p->contadorPrograma;
}

void mGOTO(processInfo* p, char* valor) {
	int i;
	/*En el caso de que solo tenga que moverse una cantidad de líneas*/
	char* valCambiar = strtok(valor, "$");
	int c = atoi(valCambiar);
	if (c != 0) {
		p->contadorPrograma += c;
	} else {
		/*En el caso de que no se encuetre un $*/
		int cTemp = 0;
		char* lTemp = calloc(1, sizeof(char));
		lTemp = strtok(p->archivoInstrucciones[cTemp], " ");
		while ((cTemp < p->cantidadDeLineas) && (strcmp(lTemp, valor) != 0)) {
			cTemp++;
			lTemp = strtok(p->archivoInstrucciones[cTemp], " ");
	    }
	
		if (strcmp(lTemp, valor) == 0) {
			p->contadorPrograma = cTemp;
		}
	}
	++p->contadorPrograma;
}

void mIORLW(processInfo* p, char* valor) {
	if (strcmp(valor, "STATUS") == 0) {
		orRegistros(p->STATUS, p->w);
	} else if (strcmp(valor, "TRISA") == 0) {
		orRegistros(p->TRISA, p->w);
	} else if (strcmp(valor, "PORTA") == 0) {
		orRegistros(p->PORTA, p->w);
	} else if (strcmp(valor, "PORTB") == 0) {
		orRegistros(p->PORTB, p->w);
	}
	++p->contadorPrograma;
}

void mSUBLW(processInfo* p, char* valor) { 
	if (strcmp(valor, "PORTA") == 0) {
		restarRegistros(p->PORTA, p->w);
	} else if (strcmp(valor, "PORTB") == 0) {
		restarRegistros(p->PORTB, p->w);
	} else if (strcmp(valor, "STATUS") == 0) {
		restarRegistros(p->STATUS, p->w);
	} else if (strcmp(valor, "TRISA") == 0) {
		restarRegistros(p->TRISA, p->w);
	} else if (strcmp(valor, "TRISB") == 0) {
		restarRegistros(p->TRISB, p->w);
	}
	++p->contadorPrograma;
}

void mADDWF(processInfo* p, char* valor) { 
	int i;
	int c = 0;

	char* reg1 = calloc(1, sizeof(char));
	char* reg2 = calloc(1, sizeof(char));

	reg1 = strtok(valor, ",");
	reg2 = strtok(NULL, ",");

	if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->PORTA, p->PORTB);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->PORTA, p->TRISA);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->PORTA, p->TRISB);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->PORTA, p->STATUS);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->PORTB, p->PORTA);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->PORTB, p->TRISA);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->PORTB, p->TRISB);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->PORTB, p->STATUS);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->TRISA, p->PORTA);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->TRISA, p->PORTB);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->TRISA, p->TRISB);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->TRISA, p->STATUS);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->TRISB, p->PORTA);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->TRISB, p->PORTB);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->TRISB, p->TRISA);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->TRISB, p->STATUS);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->STATUS, p->PORTA);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->STATUS, p->PORTB);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->STATUS, p->TRISA);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->STATUS, p->TRISB);
	}
	++p->contadorPrograma;
}

void mANDWF(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTA") == 0) {
		andFRegistros(p->PORTA, p->f);
	} else if (strcmp(valor, "PORTB") == 0) {
		andFRegistros(p->PORTB, p->f);
	} else if (strcmp(valor, "STATUS") == 0) {
		andFRegistros(p->STATUS, p->f);
	} else if (strcmp(valor, "TRISA") == 0) {
		andFRegistros(p->TRISA, p->f);
	} else if (strcmp(valor, "TRISB") == 0) {
		andFRegistros(p->TRISB, p->f);
	}
	++p->contadorPrograma;
}

void mMOVF(processInfo* p, char* valor) {
	char* valReg1 = calloc(1, sizeof(char));
	char* valReg2 = calloc(1, sizeof(char));

	valReg1 = strtok(valor, ",");
	valReg2 = strtok(NULL, ",");

	if ((strcmp(valReg1, "PORTA") == 0) && (strcmp(valReg2, "PORTB") == 0)) {
		copiarRegistro(p->PORTB, p->PORTA);
	} else if ((strcmp(valReg1, "PORTA") == 0) && (strcmp(valReg2, "STATUS") == 0)) {
		copiarRegistro(p->STATUS, p->PORTA);
	} else if ((strcmp(valReg1, "PORTA") == 0) && (strcmp(valReg2, "TRISA") == 0)) {
		copiarRegistro(p->TRISA, p->PORTA);
	} else if ((strcmp(valReg1, "PORTB") == 0) && (strcmp(valReg2, "PORTA") == 0)) {
		copiarRegistro(p->PORTA, p->PORTB);
	} else if ((strcmp(valReg1, "PORTB") == 0) && (strcmp(valReg2, "STATUS") == 0)) {
		copiarRegistro(p->STATUS, p->PORTB);
	} else if ((strcmp(valReg1, "PORTB") == 0) && (strcmp(valReg2, "TRISA") == 0)) {
		copiarRegistro(p->TRISA, p->PORTB);
	} else if ((strcmp(valReg1, "STATUS") == 0) && (strcmp(valReg2, "PORTA") == 0)) {
		copiarRegistro(p->PORTA, p->STATUS);
	} else if ((strcmp(valReg1, "STATUS") == 0) && (strcmp(valReg2, "PORTB") == 0)) {
		copiarRegistro(p->PORTB, p->STATUS);
	} else if ((strcmp(valReg1, "STATUS") == 0) && (strcmp(valReg2, "TRISA") == 0)) {
		copiarRegistro(p->TRISA, p->STATUS);
	} else if ((strcmp(valReg1, "TRISA") == 0) && (strcmp(valReg2, "PORTA") == 0)) {
		copiarRegistro(p->PORTA, p->TRISA);
	} else if ((strcmp(valReg1, "TRISA") == 0) && (strcmp(valReg2, "PORTB") == 0)) {
		copiarRegistro(p->PORTB, p->TRISA);
	} else if ((strcmp(valReg1, "TRISA") == 0) && (strcmp(valReg2, "STATUS") == 0)) {
		copiarRegistro(p->STATUS, p->TRISA);
	} else if ((strcmp(valReg1, "TRISA") == 0) && (strcmp(valReg2, "TRISB") == 0)) {//*********
		copiarRegistro(p->TRISB, p->TRISA);
	} else if ((strcmp(valReg1, "PORTA") == 0) && (strcmp(valReg2, "TRISB") == 0)) {
		copiarRegistro(p->TRISB, p->PORTA);
	} else if ((strcmp(valReg1, "PORTB") == 0) && (strcmp(valReg2, "TRISB") == 0)) {
		copiarRegistro(p->TRISB, p->PORTB);
	} else if ((strcmp(valReg1, "STATUS") == 0) && (strcmp(valReg2, "TRISB") == 0)) {
		copiarRegistro(p->TRISB, p->STATUS);
	} else if ((strcmp(valReg1, "TRISB") == 0) && (strcmp(valReg2, "PORTB") == 0)) {
		copiarRegistro(p->PORTB, p->TRISB);
	} else if ((strcmp(valReg1, "TRISB") == 0) && (strcmp(valReg2, "STATUS") == 0)) {
		copiarRegistro(p->STATUS, p->TRISB);
	} else if ((strcmp(valReg1, "TRISB") == 0) && (strcmp(valReg2, "TRISA") == 0)) {
		copiarRegistro(p->TRISA, p->TRISB);
	} else if ((strcmp(valReg1, "TRISB") == 0) && (strcmp(valReg2, "PORTA") == 0)) {
		copiarRegistro(p->PORTA, p->TRISB);
	}
	++p->contadorPrograma;
}

void mSUBWF(processInfo* p, char* valor) { //en desarrollo
	char* reg1 = calloc(1, sizeof(char));
	char* reg2 = calloc(1, sizeof(char));

	reg1 = strtok(valor, ",");
	reg2 = strtok(NULL, ",");

	if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->PORTA, p->PORTB);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->PORTA, p->TRISA);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->PORTA, p->TRISB);
	} else if (strcmp(reg1, "PORTA") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->PORTA, p->STATUS);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->PORTB, p->PORTA);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->PORTB, p->TRISA);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->PORTB, p->TRISB);
	} else if (strcmp(reg1, "PORTB") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->PORTB, p->STATUS);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->TRISA, p->PORTA);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->TRISA, p->PORTB);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->TRISA, p->TRISB);
	} else if (strcmp(reg1, "TRISA") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->TRISA, p->STATUS);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->TRISB, p->PORTA);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->TRISB, p->PORTB);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->TRISB, p->TRISA);
	} else if (strcmp(reg1, "TRISB") == 0 && strcmp(reg2, "STATUS") == 0) {
		sumarRegistros(p->w, p->TRISB, p->STATUS);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "PORTA") == 0) {
		sumarRegistros(p->w, p->STATUS, p->PORTA);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "PORTB") == 0) {
		sumarRegistros(p->w, p->STATUS, p->PORTB);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "TRISA") == 0) {
		sumarRegistros(p->w, p->STATUS, p->TRISA);
	} else if (strcmp(reg1, "STATUS") == 0 && strcmp(reg2, "TRISB") == 0) {
		sumarRegistros(p->w, p->STATUS, p->TRISB);
	}
	++p->contadorPrograma;
}

void mBCF(processInfo* p, char* valor) {
	/*Escribe un 0, en la posición de un registro*/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	valReg = strtok(valor, ",");
	valPos = strtok(NULL, ",");
	int posicion = atoi(valPos);
	if (strcmp(valReg, "STATUS")) {
		p->STATUS[posicion] = 0;
	} else if (strcmp(valReg, "PORTA") == 0) {
		p->PORTA[posicion] = 0;
	} else if (strcmp(valReg, "PORTB") == 0) {
		p->PORTB[posicion] = 0;
	} else if (strcmp(valReg, "TRISA") == 0) {
		p->TRISA[posicion] = 0;
	}
	++p->contadorPrograma;
}

void mBSF(processInfo* p, char* valor) {
	/*Escribe un 1, en la posición de un registro*/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	valReg = strtok(valor, ",");
	valPos = strtok(NULL, ",");
	int posicion = atoi(valPos);

	if (strcmp(valReg, "STATUS")) {
		p->STATUS[posicion] = 1;
	} else if (strcmp(valReg, "PORTA") == 0) {
		p->PORTA[posicion] = 1;
	} else if (strcmp(valReg, "PORTB") == 0) {
		p->PORTB[posicion] = 1;
	} else if (strcmp(valReg, "TRISA") == 0) {
		p->TRISA[posicion] = 1;
	}
	++p->contadorPrograma;
}

void mCALL(processInfo* p, char* valor) {
	int cTemp = 0;
	char* lTemp = calloc(1, sizeof(char));
	lTemp = strtok(p->archivoInstrucciones[cTemp], " ");
	while ((cTemp < p->cantidadDeLineas) && (strcmp(lTemp, valor) != 0)) {
		cTemp++;
	}
	if (strcmp(lTemp, valor) == 0) {
		p->contadorPrograma = cTemp;
	}
	++p->contadorPrograma;
}

void mRETURN(processInfo* p, char* valor) {
	int cTemp = 0;
	char* lTemp = calloc(1, sizeof(char));
	lTemp = strtok(p->archivoInstrucciones[cTemp], " ");
	while ((cTemp < p->cantidadDeLineas) && (strcmp(lTemp, valor) != 0)) {
		cTemp++;
	}
	if (strcmp(lTemp, valor) == 0) {
		p->contadorPrograma = cTemp;
	}
	++p->contadorPrograma;
}

void mBTFSC(processInfo* p, char* valor) {
	/*
	* Si el registro en la posición dada es 0 la siguiente instrucción
	* es ejecutada.
	*/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	valReg = strtok(valor, ",");
	valPos = strtok(NULL, ",");
	int posicion = atoi(valPos);
	if (strcmp(valReg, "STATUS")) {
		if (p->STATUS[posicion] == 0)
			p->contadorPrograma++;
	} else if (strcmp(valReg, "PORTA") == 0) {
		if (p->PORTA[posicion] == 0)
			p->contadorPrograma++;
	} else if (strcmp(valReg, "PORTB") == 0) {
		if (p->PORTB[posicion] == 0)
			p->contadorPrograma++;
	} else if (strcmp(valReg, "TRISA") == 0) {
		if (p->TRISA[posicion] == 0)
			p->contadorPrograma++;
	}
}

void mMOVLW(processInfo* p, char* valor) {
	int i;
	char* temp = calloc(1, sizeof(char));
	
	temp = strtok(valor, "'");   
	temp = strtok(NULL, "'");    
	
	/*Lo convierte a entero para quitarle los ceros a la izquierda*/
	int v = atoi(temp);
	
	char* buffer = calloc(1, sizeof(char));
	char* tempValor = calloc(1, sizeof(char));

	tempValor = strdup(buffer);

	int sizeTempValor = strlen(tempValor);
	int c = sizeTempValor - 1;

	for (i = 31; i >= 0 ; --i)
	{
		if (c >= 0) {
			p->w[i] = tempValor[i] - '0';
		} else {
			p->w[i] = 0;
		}
		--c;
	}
	/**/
	++p->contadorPrograma;
}

/*Suma a y b, y resultado se guarda en c*/
void sumarRegistros(int* a, int* b, int* c) {
	int acarreo = 0;
	for (int i = 31; i >= 0; --i)
	{
		if (a != b){
	   		if (acarreo == 1) {
	   			c[i] = 0;
	   			acarreo= 1;
	       	} else if (acarreo == 0) {
	       		c[i] = 1;
	       		acarreo = 0;  
	       	}
	   	}else{
	   		if (a == 0) {
	       		if (acarreo == 1) {
	       			c[i] = 1;
	       			acarreo = 0;   
	       		} else {
	       			c[i] = 0;
	      			acarreo = 0;
	           	}
	       	}else {
	       		if (acarreo ==1) {
	       			c[i] = 1;
	       			acarreo = 1;
	       		} else {
	      			c[i] = 0;
	       			acarreo = 1;
	       		}
	   		}	
	   	}
	}
}

void limpiarRegistro(int* a) {
	for (int i = 0; i < BITS; ++i)
	{
		a[i] = 0;
	}
}

void copiarRegistro(int* dest, int* src) {
	for (int i = 0; i < BITS; ++i)
	{
		dest[i] = src[i];
	}
}

void andRegistros(int* a, int* b) {
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] && b[i]) {
			b[i] = 1;
		} else {
			b[i] = 0;
		}
	}
}

void orRegistros(int* a, int* b) {
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] || b[i]) {
			b[i] = 1;
		} else {
			b[i] = 0;
		}
	}
}

void restarRegistros(int* a, int* b) {
	int j = 0;
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] - b[i] < 0) {
			/*busca un un 1*/
			j = i;
			while (a[j] != 1) {
				j++;
			}
			a[i] = a[j] - b[i];
			a[j] = 0;
		} else {
			a[i] -= b[i];
		}
	}
}

void andFRegistros(int* a, int* b) {
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] && b[i]) {
			a[i] = 1;
		} else {
			a[i] = 0;
		}
	}
}

/*Resta a y b, y lo guarda en C*/
void restarFRegistros(int* b, int* a, int* c) {
	int j = 0;
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] - b[i] < 0) {
			/*busca un un 1*/
			j = i;
			while (a[j] != 1) {
				j++;
			}
			c[i] = a[j] - b[i];
			c[j] = 0;
		} else {
			c[i] = a[i] - b[i];
		}
	}
}