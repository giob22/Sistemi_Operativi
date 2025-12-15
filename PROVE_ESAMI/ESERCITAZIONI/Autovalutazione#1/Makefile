all: main_exe generatore_exe analizzatore_exe elaboratore_exe

main_exe: main.o semafori.o
	gcc -o main_exe semafori.o main.o
main.o: main.c semafori.h
	gcc -c main.c
generatore_exe:	generatore.o semafori.o
	gcc -o generatore_exe generatore.o semafori.o
analizzatore_exe: analizzatore.o semafori.o
	gcc -o analizzatore_exe analizzatore.o semafori.o
elaboratore_exe: elaboratore.o semafori.o
	gcc -o elaboratore_exe elaboratore.o semafori.o

generatore.o: generatore.c semafori.h
	gcc -c generatore.c
elaboratore.o: elaboratore.c semafori.h
	gcc -c elaboratore.c
analizzatore.o: analizzatore.c semafori.h
	gcc -c analizzatore.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm *_exe
	rm *.o