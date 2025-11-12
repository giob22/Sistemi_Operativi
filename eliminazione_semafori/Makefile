all: programma1_exe programma2_exe

programma1_exe: programma1.o semafori.o
	gcc -o programma1_exe programma1.o semafori.o


programma2_exe: programma2.o semafori.o
	gcc -o programma2_exe programma2.o semafori.o


programma1.o: programma1.c semafori.h
	gcc -c programma1.c
programma2: programma2.c semafori.h
	gcc -c programma2.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c


clean:
	rm *.o
	rm *_exe