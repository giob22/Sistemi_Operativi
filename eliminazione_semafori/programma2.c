#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "semafori.h"


int main(){
	// inizzializzo un semaforo che verrá condiviso tra due processi
	key_t k_sem = ftok("./programma1.c", 'b');
	printf("chiave: %d\n", (int)k_sem);

	// dopo aver ottenuto la chiave dell'array semaforico ottengo il costruttore
	// che dovrebbe già esser creato dal programma1
	
	int ds_sem = semget(k_sem, 1,0);
	printf("valore di ds_sem: %d\n", ds_sem);
	if (ds_sem < 0){
		printf("il semaforo é stato creato correttamente dal programma1.c\n");
		ds_sem = semget(k_sem, 1, 0);
		printf("ho ottenuto il semaforo esistente\n");
	}else{
		printf("il semaforo non é stato creato, ERRORE");
		exit(1);
	}
	printf("[PROGRAMMA 2] il descrittore del semaforo é: %d\n", ds_sem);


	// potrei utilizzare il semaforo, ma non lo faccio, attendo 10 secondi per vedere cosa accade
	// se il programma1.c lo marca come eliminabile.
	// per vedere se é stato eliminato in questo arco di tempo controllo ipcs per vedere
	// se esiste ancora il semaforo
	// prima di attendere risveglio il semaforo il programma precedente in attesa sul semaforo
	// cosí piú nessun processo é sulla cosa di attesa di questo
	// struct sembuf op;
	// op.sem_num = 0;
	// op.sem_op = 1;
	// op.sem_flg = 0;
	Signal_sem(ds_sem, 0);
	printf("ho segnalato il programma1 bloccato sul semaforo\n");

	// semop(ds_sem, &op, 1);

	
	sleep(10);
	// controllo ipcs


	// questi sono i passi che dovrá fare anche il secondo programma per 
	// ottenere il descrittore del semaforo.

	
	



	return 0;
}
