#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "procedure.h"


int main() {

	int id_queue, id_shm;
	key_t chiave_shm = IPC_PRIVATE;// TODO
	key_t chiave_mbx = IPC_PRIVATE;// TODO

	id_shm = shmget(chiave_shm, sizeof(MonitorRisultati), IPC_CREAT | 0664);//TODO
	id_queue = msgget(chiave_mbx, IPC_CREAT | 0664);// TODO


	MonitorRisultati * ls = shmat(id_shm, NULL, 0);
	if(ls==(void*)-1){
		perror("Errore SHMAT");
		exit(1);
	}

	// TODO: inizializzare struttura

	ls->coda = 0;
	ls->testa = 0;
	ls->dim = 0;
	init_monitor(&(ls->m), 2);
	
	
	pid_t pid;

	int j;
	for(j=0; j < N_STAMPA ; j++) {
		
		// TODO
		pid = fork();
		if (pid == 0) {
			printf("[%d]Inizio Stampa\n",getpid());
			stampa(ls);
			exit(0);
		}
						
	}
	for(j=0; j < N_FILTRI ; j++) {
		pid = fork();	
		if (pid == 0) {
			filtro(ls, id_queue);
			exit(0);
		}	
	}

	for(j=0; j < N_GENERATORI ; j++) {
	
		// TODO
		pid = fork();
		if (pid == 0) {
			generazione(id_queue);
			exit(0);
		}
	}

	// TODO: attendere terminazione processi figli
	for(int i = 0; i < N_GENERATORI + N_STAMPA + N_FILTRI; i++){
		wait(NULL);
	}
	
	// TODO: deallocazione strutture
	remove_monitor(&(ls->m));
	msgctl(id_queue, IPC_RMID, NULL);
	shmctl(id_shm, IPC_RMID, NULL);
}





