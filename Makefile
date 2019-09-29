all: ejecutable

ejecutable: main.o Proceso.o  Nodo.o Lista.o ArchivoBusiness.o ArchivoData.o Interprete.o
	$(CC) -o ejecutable *.o `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lpthread
main.o:
	$(CC) -c main.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
Proceso.o:
	$(CC) -c Proceso.h Proceso.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
Nodo.o:
	$(CC) -c ADT/Nodo.h ADT/Nodo.c 
Lista.o:
	$(CC) -c ADT/Lista.h ADT/Lista.c
ArchivoBusiness.o:
	$(CC) -c ArchivoBusiness.h ArchivoBusiness.c
ArchivoData.o:
	$(CC) -c ArchivoData.h ArchivoData.c
Interprete.o:
	$(CC) -c Interprete.h Interprete.c
run:
	./ejecutable
clean:
	rm -rf main.o Proceso.o Interprete.o ejecutable *.h.gch
	clear