#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "header.h"

void generatore_testo(char text[], int dim){
	for (int i = 0; i < dim - 1; i++){
		text[i] = 'a' + rand()%26;
	}
	text[dim-1] = '\0';
}


int main(){
	// creo la message queue su cui si scambieranno i messaggi i processi
	key_t key = IPC_PRIVATE;
	int st;
	int msq_id = msgget(key, IPC_CREAT | 0664);
	#ifdef DEBUG
		master("ho creato la message queue");
	#endif
	// inizializzo la comunicazione sincrona 
	#ifdef DEBUG
		master("eseguo la init_synch()");
	#endif
	init_synch();

	// creo i due processi che si messaggieranno tramite questo protocollo (RENDEZVOUS)
	pid_t pid;

	pid = fork();
	if ( pid == 0 ){
		printf("[P] attivato\n");
		srand(time(NULL) * getpid());
		char text[DIM_MSG];
		generatore_testo(text, DIM_MSG);
		produci(msq_id, text);
		exit(0);
	}
	pid = fork();
	if ( pid == 0 ){
		printf("[C] attivato\n");
		consuma(msq_id);
		exit(0);
	}


	for (int i = 0; i < 2; i++){
		pid = wait(&st);
		printf("Processo (%d) e\' terminato con stato di terminazione %d\n",pid ,WEXITSTATUS(st));	
	}

	
	#ifdef DEBUG
		master("elimino la message queue e eseguo la leave_synch()");
	#endif
	msgctl(msq_id, IPC_RMID, 0);
	leave_synch();

	return 0;
}
