all: start p1 p2 p3

procedure.o: procedure.c header.h
	gcc -c procedure.c

start: start.o procedure.o
	gcc -o start start.o procedure.o

p1: p1.o procedure.o
	gcc -o p1 procedure.o p1.o

p2: p2.o procedure.o
	gcc -o p2 procedure.o p2.o

p3: p3.o procedure.o
	gcc -o p3 procedure.o p3.o

p1.o: p1.c header.h
	gcc -o p1.o -c p1.c
p2.o: p2.c header.h
	gcc -o p2.o -c p2.c
p3.o: p3.c header.h
	gcc -o p3.o -c p3.c


clean:
	rm *.o
	rm start p2 p1 p3