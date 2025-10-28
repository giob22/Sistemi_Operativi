#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#include "semafori.h"

void randomize(int arr[],int n){
	srand(time(NULL));
	
	for(int i = 0; i < n; i++){
		arr[i] = rand()%10000;
	
	}
	
	
}

int main(){
	// creiamo un array di interi random
	int n = 10000;
	int arr[n];
	randomize(arr, n);
	
	// creo la shm dove ogni processo inserirá il proprio minimo se é minore del minomo corrente.
	key_t k_shm = IPC_PRIVATE;
	// ottengo il descrittore della shm
	int id_shm = shmget(k_shm, sizeof(int) ,IPC_CREAT | 0644);

	// collego la shm ad un puntatore
	int *p = shmat(id_shm, NULL, 0);
	// lo inizializzo a -1 perché i valori nell'array sono interi positivi
	*p = -1;

	// creo la key per il semaforo per gestire la mutua eclusione sulla shm
	key_t k_sem = IPC_PRIVATE;
	// ottengo il descrittore del semaphor set
	int id_sem = semget(k_sem, 1, IPC_CREAT | 0664);

	// inizializzo il valore dei semafori (in questo caso 1) del semaphor set
	int res = semctl(id_sem, 0, SETVAL, 1); // ho inizializzato un semaforo binario
	if (res < 0)	
	{
		printf("errore nell'inizializzazione del semaforo\n");
	}
	// creo 10 figli che dovrenno ognuno avente una porzione di array di 1000 elementi e dovranno ricercare il minimo.
	
	int par = 1000;
	for (int i = 0; i < 10; i++)
	{
		pid_t pid = fork();
		if (pid == 0) // Sono nel contesto dei figli
		{
			int min = arr[1+ par*i];
			// ad ogni figlio spetta una partizione di array di 1000 elementi su cui operare
			for (int j = 1 + par*i; j < par + par * i; j++)
			{
				if (min > arr[j])
				{
					min = arr[j];
				}
				
			}
			printf("[FIGLIO %d pid: %d] il minimo trovato é: %d\n", i, getpid(),min);

			// una volta che i figli hanno trovato il minimo dovranno accedere alla shm per inserirlo
			// ovviamente prima di inserirlo devono controllare che il minimo locale sia minore del minimo corrente
			// questo operazione deve esser fatta in MUTUA ESCLUSIONE, altrimenti incorreremo in race condition


			// sezione critica
			wait_sem(id_sem, 0);
			if (*p == -1 || *p > min)
			{
				*p = min;
			}
			signal_sem(id_sem,0);

			exit(0);
		}
			
	}
	for (int i = 0; i < par; i++)
	{
		wait(NULL);
	}
	printf("[PADRE %d] il valore minimo: %d\n", getpid(),*p);
	

	// marco la shm come eliminabile per il kernel
	// marco il semaphor set come eliminabile per il kernel
	// entrambi verrano poi eliminati nel momento in cui più nessun processo è ad essi collegato.
	shmctl(id_shm, IPC_RMID, 0);
	semctl(id_sem,0,IPC_RMID);
	

	return 0;
}
