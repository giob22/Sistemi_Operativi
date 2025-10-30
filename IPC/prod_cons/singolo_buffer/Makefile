all: singleBuffer_consprod

singleBuffer_consprod: main.o semafori.o procedure.o
	gcc -o singleBuffer_consprod main.o semafori.o procedure.o

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

main.o: procedure.h semafori.h main.c
	gcc -c main.c

clean:
	rm -r *.o
	rm -r singleBuffer_consprod
