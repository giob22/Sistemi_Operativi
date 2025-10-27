#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
// libreria per la gestione di semafori includo all'inderno della libreria semafori.h in cui implemento la wait e signal
// #include <sys/sem.h>

#include "semafori.h"


int main(){

	key_t  k_shmem = IPC_PRIVATE;
	int ds_shmem = shmget(k_shmem, sizeof(int), IPC_CREAT | 0644);

	int *ptr = (int *)shmat(ds_shmem, NULL, 0);

    // creiamo l'array semaforico di un semaforo
    key_t k_sem = IPC_PRIVATE;

    int id_sem = semget(k_sem, 1, IPC_CREAT | 0644);
    // inizializziamo il semaforo
    semctl(id_sem, 0, SETVAL, 1); // sem binario aka LOCK

    

	*ptr = 0;
	// creo due processi figli che incrementano in modo concorrente 100
	// volte la variabile puntata da ptr
	for (int i = 0; i < 2; i++){
		pid_t pid = fork();
		if(pid == 0){
			printf("[FIGLIO %d] sto per incrementare 10000 volte \n", getpid());
			
			for (int j = 0; j < 10000; j++){
				// sezione critica che proteggiamo con le primitive wait() e signal()
                Wait_sem(id_sem, 0);
				(*ptr)++;
                Signal_sem(id_sem,0);
			}
			exit(0);
		
		}
	
	}
	for (int i = 0; i < 2; i++){
		wait(NULL);
	}
	printf("[PADRE %d] il valore finale della shm %d\n",getpid(),*ptr);

    // oltre a marcare come eliminabile la shm, facciamolo anche per il semaforo appena creato
	shmctl(ds_shmem, IPC_RMID, 0);
    semctl(id_sem,0,IPC_RMID);


	return 0;
}