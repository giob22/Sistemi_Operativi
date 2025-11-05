all: Lettori_Scrittori

Lettori_Scrittori: semafori.o mainSL.o procedure.o
	gcc -o Lettori_Scrittori_exe mainSL.o procedure.o semafori.o
semafori.o: ../../sem/semafori.h ../../sem/semafori.c
	gcc -c ../../sem/semafori.c
procedure.o: procedure.h procedure.c
	gcc -c procedure.c

clean:
	rm Lettori_Scrittori_exe
	rm *.o