#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok("./main.c", 'a')/*TODO*/;

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT | 0664) /*TODO*/;

	struct Coda * c = shmat(ds_shm, NULL, 0);
	
	c->coda = 0;
	c->testa = 0;

	
	//ottenimento array semafori

	key_t key_sem;
	key_sem = ftok("./main.c", 'b');

	int ds_sem = semget(key_sem, 4, IPC_CREAT | 0664);
	
	//TODO inizializzazione coda e semafori

	semctl(ds_sem, SPAZIO_DISP, SETVAL, DIM_BUFFER);
	semctl(ds_sem, DATO_DISP, SETVAL, 0);
	semctl(ds_sem, MUTEXP, SETVAL, 1);
	semctl(ds_sem, MUTEXC, SETVAL, 1);

  
	int i = 0;
	int pid;
	int status;
	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		sleep(1);

		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				//TODO aggiungere exec per avviare il server
				execl("./server", "server", NULL);
				perror("Errore nella sostituzione del codice per server");
				exit(-1);
			}else{
				//TODO aggiungere exec per avviare il client
				execl("./client", "client", NULL);
				perror("Errore nella sostituzione del codice per client");
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	//TODO rimozione array semafori e memoria condivisa

	shmctl(ds_shm, IPC_RMID, NULL);
	semctl(ds_sem, 4, IPC_RMID);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}
