all: start

debug: start_debug

start_debug: main_debug.o procedure_debug.o
	gcc -pthread -o start_debug main_debug.o procedure_debug.o
main_debug.o: main.c header.h
	gcc -c -DDEBUG main.c -o main_debug.o
procedure_debug.o: procedure.c header.h
	gcc -c -DDEBUG procedure.c -o procedure_debug.o



start: main.o procedure.o
	gcc -pthread -o start main.o procedure.o
main.o: main.c header.h
	gcc -c main.c
procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	-rm *.o 
	-rm star* 
