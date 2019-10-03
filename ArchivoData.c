#include "ArchivoData.h"

void leer(processInfo* p, char* ruta) {
	FILE *f;
   char c;
   char d;
   int e;
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

   e = 0;
   for (int i = 0; i < lineas; ++i)
   {
   	fgets(temp, 1024, f);
      if (isalpha(temp[0]) != 0) {
         p->archivoInstrucciones[e++] = strdup(temp);
      } else {
         --p->cantidadDeLineas;
      }
   }
   fclose(f);
}