#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>


int main(){

	key_t  k_shmem = IPC_PRIVATE;
	int ds_shmem = shmget(k_shmem, sizeof(int), IPC_CREAT | 0644);

	int *ptr = (int *)shmat(ds_shmem, NULL, 0);

	*ptr = 0;
	// creo due processi figli che incrementano in modo concorrente 100
	// volte la variabile puntata da ptr
	for (int i = 0; i < 2; i++){
		pid_t pid = fork();
		if(pid == 0){
			printf("[FIGLIO %d] sto per incrementare 10000 volte \n", getpid());
			
			for (int j = 0; j < 10000; j++){
				// sezione critica che non viene eseguita in mutua esclusione
				(*ptr)++;
			}
			exit(0);
		
		}
	
	}
	for (int i = 0; i < 2; i++){
		wait(NULL);
	}
	printf("[PADRE %d] il valore finale della shm %d\n",getpid(),*ptr);

	shmctl(ds_shmem, IPC_RMID, 0);


	return 0;
}
