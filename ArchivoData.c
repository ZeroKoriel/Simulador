#include "ArchivoData.h"

void leer(processInfo* p, char* ruta) {
	FILE *f;
   char c;
   char temp[1024];
   int lineas = 0;
   if ((f = fopen(ruta, "r")) == NULL){
   	perror(ruta);
   }
 
   while((c = fgetc(f)) != EOF) {
   	if (c == '\n'){
     	lineas++;
   	}
   }
 
   rewind(f);

   p->cantidadDeLineas = lineas;
   p->archivoInstrucciones = calloc(lineas, sizeof(char*));

   for (int i = 0; i < lineas; ++i)
   {
   	fgets(temp, 1024, f);
   	p->archivoInstrucciones[i] = strdup(temp);
   }
   fclose(f);
}