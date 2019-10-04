#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <math.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "DataTypes.h"

void ejecutarInstruccion(processInfo*);

void buscarLinea(processInfo*, char*);
void comprobarNemotecnico(processInfo*, char*, char*);

/*funciones de apoyo*/
void limpiarRegistro(int*);
void copiarRegistro(int*, int*);

void andRegistros(int*, int*, int);
void orRegistros(int*, int*);
void sumarRegistros(int*, int*, int);
void restarRegistros(int*, int*, int);

void imprimir(processInfo*);

/*funciones de los nemónicos*/
void mCLRF(processInfo*, char*);
void mCLRW(processInfo*);
void mMOVWF(processInfo*, char*);
void mADDLW(processInfo*, char*);
void mANDLW(processInfo*, char*);
void mGOTO(processInfo*, char*);
void mIORLW(processInfo*, char*);
void mSUBLW(processInfo*, char*);
void mADDWF(processInfo*, char*);
void mANDWF(processInfo*, char*);
void mMOVF(processInfo*, char*);
void mSUBWF(processInfo*, char*);
void mBCF(processInfo*, char*);
void mBSF(processInfo*, char*);
void mCALL(processInfo*, char*);
void mRETURN(processInfo*, char*);
void mBTFSC(processInfo*, char*);
void mBTFSS(processInfo*, char*);
void mMOVLW(processInfo*, char*);

#endif