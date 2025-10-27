sem1: sem_es
	./sem_es
sem_es: semafori.o es_sem.o
	gcc -o sem_es semafori.o es_sem.o
es_sem.o: es_sem.c semafori.h
	gcc -c es_sem.c -o es_sem.o
semafori.o: semafori.h semafori.c
	gcc -c semafori.c -o semafori.o

shm1: shm_es
	echo "digita './shm_es [r]/[w {text}]'"
shm_es:
	gcc -o shm_es es_shm.c


clean:
	rm *.o
	rm *_es
