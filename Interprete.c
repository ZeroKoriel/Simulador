#include "Interprete.h"
//listo
void ejecutarInstruccion(processInfo* proceso) {
	if (proceso->contadorPrograma >= proceso->cantidadDeLineas || proceso->contadorPrograma < 0) {
		return; 
	} 
	char line[100];
	memcpy(line, proceso->archivoInstrucciones[proceso->contadorPrograma],
	 strlen(proceso->archivoInstrucciones[proceso->contadorPrograma])+1);
	buscarLinea(proceso, line);
}

//listo
void buscarLinea(processInfo* p, char* line) {
	char* nemoTemp = calloc(1, sizeof(char));
	char* valorTemp = calloc(1, sizeof(char));

	nemoTemp = strtok(line, " ");
	valorTemp = strtok(NULL, " ");

	int n=strcmp(nemoTemp," ");

	if (strcmp(nemoTemp," ")>0) {
		comprobarNemotecnico(p, nemoTemp, valorTemp);
	} else {
		while (strcmp(nemoTemp," ")<=0) {
			++p->contadorPrograma;
			char linea[100];
			memcpy(linea, p->archivoInstrucciones[p->contadorPrograma],
				strlen(p->archivoInstrucciones[p->contadorPrograma])+1);
			nemoTemp = strtok(linea, " ");
		}
		valorTemp = strtok(NULL, " ");
		comprobarNemotecnico(p, nemoTemp, valorTemp);
	}
}

//listo
void mCLRF(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTB\n") == 0) {
		limpiarRegistro(p->PORTB);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTA\n") == 0) {
		limpiarRegistro(p->PORTA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "STATUS\n") == 0) {
		limpiarRegistro(p->STATUS);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISA\n") == 0) {
		limpiarRegistro(p->TRISA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISB\n") == 0) {
		limpiarRegistro(p->TRISB);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mGOTO(processInfo* proceso, char* valorTemp){
	char* valorTemp2 = calloc(1, sizeof(char));
	char line[100];
	char* temp1 = calloc(1, sizeof(char));

	valorTemp2 = strtok(valorTemp, "$");

	if(atoi(valorTemp2)==0){
		for (int i = 0; i < proceso->cantidadDeLineas; ++i)
		{
			memcpy(line, proceso->archivoInstrucciones[i],
				strlen(proceso->archivoInstrucciones[i])+1);
			temp1=strtok(line," ");
			if(strcmp(temp1, valorTemp) == 0){
				proceso->contadorPrograma=i;
				break;
			}
		}
	}else{
		proceso->contadorPrograma+=atoi(valorTemp2);
	}
}//GOTO

//listo
void comprobarNemotecnico(processInfo* p, char* nemo, char* valor) {
	if (strcmp(nemo, "CLRF") == 0) {          
		mCLRF(p, valor);
		return;
	} else if (strcmp(nemo, "CLRW") == 0) {         
		mCLRW(p);
		return;
	} else if (strcmp(nemo, "MOVWF") == 0) {         
		mMOVWF(p, valor);
		return;
	} else if (strcmp(nemo, "ADDLW") == 0) { 
		mADDLW(p, valor);
		return;
	} else if (strcmp(nemo, "ANDLW") == 0) {              
		mANDLW(p, valor);
		return;
	} else if (strcmp(nemo, "GOTO") == 0) {
		mGOTO(p, valor);
		return;
	} else if (strcmp(nemo, "IORLW") == 0) {              
		mIORLW(p, valor);
		return;
	} else if (strcmp(nemo, "SUBLW") == 0) { 
		mSUBLW(p, valor);
		return;
	} else if (strcmp(nemo, "ADDWF") == 0) { 
		mADDWF(p, valor);
		return;
	} else if (strcmp(nemo, "ANDWF") == 0) {  
		mANDWF(p, valor);
		return;
	} else if (strcmp(nemo, "MOVF") == 0) {  
		mMOVF(p, valor);
		return;
	} else if (strcmp(nemo, "SUBWF") == 0) { 
		mSUBWF(p, valor);
		return;
	} else if (strcmp(nemo, "BCF") == 0) {
		mBCF(p, valor);
		return;
	} else if (strcmp(nemo, "BSF") == 0) {      
		mBSF(p, valor);
		return;
	} else if (strcmp(nemo, "CALL") == 0) {        
		mCALL(p, valor);
		return;
	} else if (strcmp(nemo, "RETURN") == 0) {      
		mRETURN(p, valor);
		return;
	} else if (strcmp(nemo, "END") == 0) {         
		p->contadorPrograma = p->cantidadDeLineas;
		return;
	} else if (strcmp(nemo, "BTFSC") == 0) {      
		mBTFSC(p, valor);
		return;
	} else if (strcmp(nemo, "MOVLW") == 0) {        
		mMOVLW(p, valor);
		return;
	} else if (strcmp(nemo, "BTFSS") == 0) {
		mBTFSS(p, valor);
		return;
	} 
		++p->contadorPrograma;	
}

//listo
void mCLRW(processInfo* p) {
	limpiarRegistro(p->w);
	++p->contadorPrograma;
}



//listo
void mADDLW(processInfo* p, char* valor) { 
	int i;
	int c = 0;
	int temp[BITS];
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



//listo
void mIORLW(processInfo* p, char* valor) {
	if (strcmp(valor, "STATUS\n") == 0) {
		orRegistros(p->w, p->STATUS);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISA\n") == 0) {
		orRegistros(p->w, p->TRISA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTA\n") == 0) {
		orRegistros(p->w, p->PORTA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTB\n") == 0) {
		orRegistros(p->w, p->PORTB);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISB\n") == 0) {
		orRegistros(p->w, p->TRISB);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mADDWF(processInfo* p, char* valor) { 
	char* reg1 = calloc(1, sizeof(char));
	char* reg2 = calloc(1, sizeof(char));

	reg1 = strtok(valor, ",");
	reg2 = strtok(NULL, ",");
	int numero = atoi(reg2);

	if (strcmp(reg1, "PORTA") == 0) {
		sumarRegistros(p->w, p->PORTA, numero);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "PORTB") == 0) {
		sumarRegistros(p->w, p->PORTB, numero);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISA") == 0) {
		sumarRegistros(p->w, p->TRISA, numero);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISB") == 0) {
		sumarRegistros(p->w, p->TRISB, numero);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "STATUS") == 0) {
		sumarRegistros(p->w, p->STATUS, numero);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mANDWF(processInfo* p, char* valor) {
	char* reg1 = calloc(1, sizeof(char));
	char* reg2 = calloc(1, sizeof(char));

	reg1 = strtok(valor, ",");
	reg2 = strtok(NULL, ",");
	int v = atoi(reg2);

	if (strcmp(reg1, "PORTA") == 0) {
		andRegistros(p->w, p->PORTA, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "PORTB") == 0) {
		andRegistros(p->w, p->PORTB, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISA") == 0) {
		andRegistros(p->w, p->TRISA, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISB") == 0) {
		andRegistros(p->w, p->TRISB, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "STATUS") == 0) {
		andRegistros(p->w, p->STATUS, v);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mANDLW(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTA\n") == 0) {
		andRegistros(p->w, p->PORTA, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTB\n") == 0) {
		andRegistros(p->w, p->PORTB, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "STATUS\n") == 0) {
		andRegistros(p->w, p->STATUS, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISA\n") == 0) {
		andRegistros(p->w, p->TRISA, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISB\n") == 0) {
		andRegistros(p->w, p->TRISB, 0);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mMOVF(processInfo* p, char* valor) {
	char* valReg1 = calloc(1, sizeof(char));
	char* valReg2 = calloc(1, sizeof(char));

	valReg1 = strtok(valor, ",");
	valReg2 = strtok(NULL, ",");
	int v = atoi(valReg2);
	if (strcmp(valReg1, "PORTA") == 0) {
		if (v == 0)
			copiarRegistro(p->w, p->PORTA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valReg1, "PORTB") == 0) {
		if (v == 0)
			copiarRegistro(p->w, p->PORTB);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valReg1, "STATUS") == 0) {
		if (v == 0)
			copiarRegistro(p->w, p->STATUS);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valReg1, "TRISA") == 0) {
		if (v == 0)
			copiarRegistro(p->w, p->TRISA);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valReg1, "TRISB") == 0) {
		if (v == 0)
			copiarRegistro(p->w, p->TRISB);
		++p->contadorPrograma;
		return;
	} 
}

//listo
void mMOVWF(processInfo* p, char* valor) {
	if (strcmp(valor, "PORTA") == 0) {
		copiarRegistro(p->PORTA, p->w);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTB") == 0) {
		copiarRegistro(p->PORTB, p->w);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "STATUS") == 0) {
		copiarRegistro(p->STATUS, p->w);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISA") == 0) {
		copiarRegistro(p->TRISA, p->w);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISB") == 0) {
		copiarRegistro(p->TRISB, p->w);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mSUBLW(processInfo* p, char* valor) { 
	if (strcmp(valor, "PORTA\n") == 0) {
		restarRegistros(p->w,p->PORTA, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "PORTB\n") == 0) {
		restarRegistros(p->w, p->PORTB, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "STATUS\n") == 0) {
		restarRegistros(p->w, p->STATUS, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISA\n") == 0) {
		restarRegistros(p->w, p->TRISA, 0);
		++p->contadorPrograma;
		return;
	} else if (strcmp(valor, "TRISB\n") == 0) {
		restarRegistros(p->w, p->TRISB, 0);
		++p->contadorPrograma;
		return;
	}	
}

//listo
void mSUBWF(processInfo* p, char* valor) { //en desarrollo
	char* reg1 = calloc(1, sizeof(char));
	char* reg2 = calloc(1, sizeof(char));

	reg1 = strtok(valor, ",");
	reg2 = strtok(NULL, ",");
	int v = atoi(reg2); 

	if (strcmp(reg1, "PORTA") == 0 ) {
		restarRegistros(p->w, p->PORTA, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "PORTB") == 0) {
		restarRegistros(p->w, p->PORTB, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISA") == 0) {
		restarRegistros(p->w, p->TRISA, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "TRISB") == 0) {
		restarRegistros(p->w, p->TRISB, v);
		++p->contadorPrograma;
		return;
	} else if (strcmp(reg1, "STATUS") == 0) {
		restarRegistros(p->w, p->STATUS, v);
		++p->contadorPrograma;
		return;
	}
}

//listo
void mBCF(processInfo* p, char* valor) {
	/*Escribe un 1, en la posición de un registro*/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	char* temp = calloc(1, sizeof(char));
	temp = strtok(valor, ",");
	valPos = strtok(NULL, ",");

	valReg = strdup(temp);

	int posicion = 31-atoi(valPos);

	if (strcmp(valReg, "STATUS") == 0) {
		p->STATUS[posicion] = 0;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "PORTA") == 0) {
		p->PORTA[posicion] = 0;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "PORTB") == 0) {
		p->PORTB[posicion] = 0;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "TRISA") == 0) {
		p->TRISA[posicion] = 0;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "TRISB") == 0) {
		p->TRISB[posicion] = 0;
		++p->contadorPrograma;
	}
}
//listo
void mBSF(processInfo* p, char* valor) {
	/*Escribe un 1, en la posición de un registro*/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	char* temp = calloc(1, sizeof(char));
	temp = strtok(valor, ",");
	valPos = strtok(NULL, ",");

	valReg = strdup(temp);

	int posicion = 31-atoi(valPos);

	if (strcmp(valReg, "STATUS") == 0) {
		p->STATUS[posicion] = 1;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "PORTA") == 0) {
		p->PORTA[posicion] = 1;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "PORTB") == 0) {
		p->PORTB[posicion] = 1;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "TRISA") == 0) {
		p->TRISA[posicion] = 1;
		++p->contadorPrograma;
	} else if (strcmp(valReg, "TRISB") == 0) {
		p->TRISB[posicion] = 1;
		++p->contadorPrograma;
	}
}

//listo
void mCALL(processInfo* p, char* valor) {
	char line[100];
	char* temp1 = calloc(1, sizeof(char));

	for (int i = 0; i < p->cantidadDeLineas; ++i)
	{
		memcpy(line, p->archivoInstrucciones[i],
			strlen(p->archivoInstrucciones[i])+1);
		temp1=strtok(line," ");
		if(strcmp(temp1, valor) == 0){
			p->contadorPrograma=i;
			break;
		}
	}
}

//listo
void mRETURN(processInfo* p, char* valor) {
	char line[100];
	char* temp1 = calloc(1, sizeof(char));

	for (int i = 0; i < p->cantidadDeLineas; ++i)
	{
		memcpy(line, p->archivoInstrucciones[i],
			strlen(p->archivoInstrucciones[i])+1);
		temp1=strtok(line," ");
		if(strcmp(temp1, valor) == 0){
			p->contadorPrograma=i;
			break;
		}
	}
}
//listo
void mBTFSC(processInfo* p, char* valor) {
	/*
	* Si el registro en la posición dada es 0 la siguiente instrucción
	* es ejecutada.
	*/
	char* valorTemp2 = calloc(1, sizeof(char));
	char* valorTemp3 = calloc(1, sizeof(char));

	/**/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));
	
	valReg = strtok(valor, ",");
	valPos = strtok(NULL, ",");

	int posicion = 31 - atoi(valPos);
	char line[100];
	int lineaTemp = p->contadorPrograma+1;

	if (strcmp(valReg, "STATUS") == 0) {
		if(p->STATUS[posicion]==0){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "PORTA") == 0) {
		if(p->PORTA[posicion]==0){
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "PORTB") == 0) {
		if(p->PORTB[posicion]==0){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "TRISA") == 0) {
		if(p->TRISA[posicion]==0){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "TRISB") == 0) {
		if(p->TRISB[posicion]==0){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	}
}

//listo
void mBTFSS(processInfo* p, char* valor) {
	/*
	* Si el registro en la posición dada es 0 la siguiente instrucción
	* es ejecutada.
	*/
	char* valorTemp2 = calloc(1, sizeof(char));
	char* valorTemp3 = calloc(1, sizeof(char));

	/**/
	char* valReg = calloc(1, sizeof(char));
	char* valPos = calloc(1, sizeof(char));

	valReg = strtok(valor, ",");
	valPos = strtok(NULL, ",");
	
	int posicion = 31 - atoi(valPos);
	char line[100];
	int lineaTemp = p->contadorPrograma+1;
	
	if (strcmp(valReg, "STATUS") == 0) {
		if(p->STATUS[posicion]==1){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "PORTA") == 0) {
		if(p->PORTA[posicion]==1){
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "PORTB") == 0) {
		if(p->PORTB[posicion]==1){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "TRISA") == 0) {
		if(p->TRISA[posicion]==1){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	} else if (strcmp(valReg, "TRISB") == 0) {
		if(p->TRISB[posicion]==1){
	
			memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
			valorTemp2 = strtok(line, " ");

			while (strcmp(valorTemp2," ")<=0) {
				++lineaTemp;
				memcpy(line, p->archivoInstrucciones[lineaTemp],strlen(p->archivoInstrucciones[lineaTemp])+1);
				valorTemp2 = strtok(line, " ");
			}

			valorTemp3 = strtok(NULL, " ");

			++lineaTemp;
			p->contadorPrograma=lineaTemp;	
		}else{
			++p->contadorPrograma;
		}
	}
}

//listo
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

/*Suma a y b, y resultado se guarda en c, listo*/
void sumarRegistros(int* a, int* b, int c) {
	int acarreo = 0;
	int temp[BITS];
	for (int i = 31; i >= 0; --i)
	{
		if (a != b){
	   		if (acarreo == 1) {
	   			temp[i] = 0;
	   			acarreo= 1;
	       	} else if (acarreo == 0) {
	       		temp[i] = 1;
	       		acarreo = 0;  
	       	}
	   	}else{
	   		if (a == 0) {
	       		if (acarreo == 1) {
	       			temp[i] = 1;
	       			acarreo = 0;   
	       		} else {
	       			temp[i] = 0;
	      			acarreo = 0;
	           	}
	       	}else {
	       		if (acarreo ==1) {
	       			temp[i] = 1;
	       			acarreo = 1;
	       		} else {
	      			temp[i] = 0;
	       			acarreo = 1;
	       		}
	   		}	
	   	}
	}
	/*0 va a w y 1 al otro*/
	if (c) {
		copiarRegistro(b, temp);
	} else {
		copiarRegistro(a, temp);
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

void orRegistros(int* dest, int* src) {
	for (int i = 0; i < BITS; ++i)
	{
		if (dest[i] || src[i]) {
			src[i] = 1;
		} else {
			src[i] = 0;
		}
	}
}

void restarRegistros(int* a, int* b, int d) {
	int j = 0;
	int temp[BITS];
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] - b[i] < 0) {
			/*busca un 1*/
			j = i;
			while (a[j] != 1) {
				j++;
			}
			temp[i] = temp[j] - b[i];
			temp[j] = 0;
		} else {
			temp[i] -= b[i];
		}
	}

	if (d) {
		copiarRegistro(b, temp);
	} else {
		copiarRegistro(a, temp);
	}
}

void andRegistros(int* a, int* b, int d) {
	int temp[BITS];
	for (int i = 0; i < BITS; ++i)
	{
		if (a[i] && b[i]) {
			temp[i] = 1;
		} else {
			temp[i] = 0;
		}
	}
	// a seria el registro w y b el otro
	// si d es 0 se guarda en w
	if (d) {
		copiarRegistro(b, temp);
	} else {
		copiarRegistro(a, temp);
	}
}