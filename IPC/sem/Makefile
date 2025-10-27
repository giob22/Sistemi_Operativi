all: main_exe
	./main_exe
main_exe: main.o semafori.o
	gcc -o main_exe main.o semafori.o
main.o: main.c semafori.h
	gcc -c main.c -o main.o
semafori.o: semafori.c semafori.h
	gcc -c semafori.c -o semafori.o
clean:
	rm *.o
	rm main_exe