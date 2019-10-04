#ifndef ADMINISTRADORM_H
#define ADMINISTRADORM_H 

#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"

#define MAXBLOQUES 1024

static int bloquesLibres;

void iniciarMemoria();

int solicitarMemoria(Process*, int);
int liberarMemoria(Process*);

int buscarIndiceInicio(int);
int marcarOcupado(Process*, int, int);
void acomodarLista();

#endif