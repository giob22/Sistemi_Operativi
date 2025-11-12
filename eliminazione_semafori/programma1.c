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
	printf("chiave: %d\n", k_sem);


	// dopo aver ottenuto la chiave dell'array semaforico ottengo il costruttore
	int ds_sem = semget(k_sem, 1, IPC_CREAT | 0644);
	printf("[PROGRAMMA 1] il descrittore del semaforo é: %d\n", ds_sem);

	// questi sono i passi che dovrá fare anche il secondo programma per 
	// ottenere il descrittore del semaforo.

	// imposto il valore iniziale del semaforo
	semctl(ds_sem, 0, SETVAL, 1);
	// attendo sul semaforo che il programma2.c ottenga il costruttore
	// struct sembuf op;
	// op.sem_num = 0;
	// op.sem_flg = 0;
	// op.sem_op = -1;
	
	// creo il secondo processo + exec
	pid_t pid = 0;
	pid = fork();
	if (pid == 0)
	{
		execl("./programma2_exe","./programma2_exe", NULL);
		printf("ERRORE EXEC\n");
		exit(1);
	}
	printf("mi metto in attesa sul semaforo\n");
	Wait_sem(ds_sem, 0);
	
	// semop(ds_sem, &op, 1);
	// nel momento in cui viene sbloccato il programma1.c





	// marca come eliminabile il semaforo;
	semctl(ds_sem, 0, IPC_RMID);

	// attendo la terminazione del programma2.c
	wait(NULL);

	printf("FINE\n");



	return 0;
}
