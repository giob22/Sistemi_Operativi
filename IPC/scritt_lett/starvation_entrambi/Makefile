all: starvation_entrambi

starvation_entrambi: mainSE.o procedure.o semafori.o
	gcc -o starvation_entrambi mainSE.o procedure.o semafori.o
procedure.o: procedure.c procedure.h semafori.h
	gcc -c procedure.c
semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm *.o
	rm starvation_entrambi