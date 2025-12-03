all: start

start: main.o procedure.o
	gcc -pthread -o start main.o procedure.o
main.o: main.c header.h
	gcc -c main.c
procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm *.o
	rm start
