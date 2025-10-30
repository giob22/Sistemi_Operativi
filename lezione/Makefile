all: main_prod_cons

main_prod_cons: main.o semafori.o procedure.o
	gcc -o main_prod_cons main.c semafori.o procedure.o
main.o: main.c semafori.h procedure.h
	gcc -c main.c
semafori.o: semafori.c semafori.h
	gcc -c semafori.c
procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm -r *.o
	rm -r main_prod_cons