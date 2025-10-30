#include "semafori.h"
#include "procedure.h"

// gestione semafori
// gestione shm
// gestione lifecycle dei processi

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>

// eventuali fork/clone di processi figli avranno una copia esatta dell'area .text di questa applicazione

int main(){

	// istanziamo shm
	// istanziamo sem per disciplinare prod e cons sull'accesso alla single buffer
	srand(time(NULL));	

	key_t shm_key = IPC_PRIVATE;

	int ds_shm = shmget(shm_key, sizeof(int), IPC_CREAT | 0644);

	// collego la shm ad un'area di memoria nel processo
	
	int * p = shmat(ds_shm, NULL, 0); // area di memoria che viene condivisa tra tutti i processi figli
					  // poiché questi sono una copia dell'immagine del processo padre 
	// tutti i processi che ottengono la per eredità o ottenendo la chiave per la risorsa IPC potrenno accedere alla risorsa dando un indirizzo privato al processo
	// ma le scritture saranno visibili a tutti i processi che hanno fatto richiesta per tale risorsa
	// utilizzando IPC_PRIVATE prima di una fork() tutti i processi figli avranno la chiave per la IPC (shm)
	
	// creiamo l'array semaforico
	key_t sem_key = IPC_PRIVATE;
	int ds_sem = semget(sem_key, 2, IPC_CREAT | 0644);

	// inizializziamo i semafori dell'array semaforico
	// utilizzati per gestire la cooperazione tra prod e cons sulla shm
	semctl(ds_sem, MESS_DISP, SETVAL, 0);
	semctl(ds_sem, SPAZIO_DISP, SETVAL, 1);

	// creiamo il produttore
	pid_t pid = fork();
	if(pid == 0){
		printf("[PID: %d] PRODUTTORE avviato\n", getpid());
		produttore(p,ds_sem);
		exit(1);
	
	}
	
	pid = fork();
	if(pid == 0){
        	printf("[PID: %d] CONSUMATORE avviato\n", getpid());
	   	consumatore(p,ds_sem);
		exit(1);
	}

	wait(NULL);
	wait(NULL);

	printf("processo terminato ...\n");

	//marchiamo shm e sem come eliminabili
	semctl(ds_sem, 0, IPC_RMID);
	shmctl(ds_shm, IPC_RMID, NULL);


	return 0;
}

