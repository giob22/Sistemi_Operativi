#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "header.h"

int main(){

	// creo la message queue
	key_t key = IPC_CREAT;
	int msq_id = init_msq(key);

	// creo i processi che si scambieranno i messaggi
	
	pid_t pid = fork();
	if (pid == 0){
		LOGP("%d", getpid());
		produci(msq_id, "ciao"),
		exit(0);
	}
	pid = fork();
	if (pid == 0){
		LOGC("%d", getpid());
		consuma(msq_id); 
		exit(0);
	}

	wait(NULL);
	wait(NULL);

	msgctl(msq_id, IPC_RMID, 0);

	return 0;
}
