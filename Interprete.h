#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <string.h>

/*Monádicas*/
#define CLRF "CLRF"
#define CLRW "CLRW"
#define MOVWF "MOVWF"
#define ADDLW "ADDLW"
#define ANDLW "ANDLW"
#define GOTO "GOTO"
#define IORLW "IORLW"
#define SUBLW "SUBLW"

/*Diádicas*/
#define ADDWF "ADDWF"
#define ANDWF "ANDWF"
#define MOVF "MOVF"
#define SUBWF "SUBWF"
#define BCF "BCF"
#define BSF "BSF"
#define CALL "CALL"
#define RETURN "RETURN"
#define END "END"

/*
	int strcmp(const char *str1, const char *str2)

	si el valor de retorno <0 indica que str1 es menor que str2.

	si Valor devuelto> 0, indica que str2 es menor que str1.

	si Valor devuelto = 0, entonces indica que str1 es igual a str2. 
*/

/*
	char* ptr = strtok(char* str, char* delim);
*/

#endif