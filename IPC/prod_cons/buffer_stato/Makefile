all: main_con_buffer_stato

main_con_buffer_stato: main.o semafori.o procedure.o
	gcc -o main_con_buffer_stato main.c semafori.o procedure.o
main.o: main.c semafori.h procedure.h
	gcc -c main.c
semafori.o: semafori.c semafori.h
	gcc -c semafori.c
procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm -r *.o
	rm -r main_con_buffer_stato