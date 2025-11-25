all: start
	./start
start: start.o procedure.o
	gcc -o start procedure.o start.o
procedure.o: procedure.c header.h
	gcc -c procedure.c
start.o: start.c header.h
	gcc -c start.c

clean:
	rm *.o
	rm start
