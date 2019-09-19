#include "ArchivoData.h"

void leer(processInfo* p) {
	FILE *f;
   char c;
   char temp[50];
   int lineas = 0;
   if ((f = fopen("asm.txt", "r")) == NULL){
   	perror("asm.txt");
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
   	fscanf(f, "%s", temp);
   	p->archivoInstrucciones[i] = strdup(temp);
   }
   fclose(f);
}