#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


int main(){
	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok("./main.c", 'a');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT | 0664);

	struct Coda * c;
	c = shmat(ds_shm, NULL, 0);

	
	//ottenimento semafori

	key_t key_sem;
	key_sem = ftok("./main.c", 'b');

	int ds_sem = semget(key_sem, 4, IPC_CREAT | 0664);
    
	printf("[Client %d] - invio richieste...\n",getpid());
	
	int i = 0;
	int num_prenotazioni;	

	srand(time(NULL) * getpid());

	while(i < NUM_REQ){
		num_prenotazioni = rand()%5 + 1;//TODO numero casuale
		//TODO invio richiesta tramite funzione "produci"
		produci(c,ds_sem, num_prenotazioni);
		printf("[Client %d] - richiesta inviata %d\n",getpid(), num_prenotazioni);
		sleep(1);
		i++;
	}
	//TODO invio richiesta di terminazione (-1)
	produci(c,ds_sem, -1);

	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
