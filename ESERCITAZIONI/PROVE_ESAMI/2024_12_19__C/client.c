
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

void bambino(int queue_req, int queue_res) {
	int i;
	for (i=0; i < NUM_LETTERE; i++) {
		/*
        	TODO: invio del messaggio
    	*/
		MessaggioRichiesta req;
		MessaggioRisposta res;
		req.type = getpid();
		
		req.numero_regali = rand() % 5 + 1;
		printf("Invio lettera a Babbo Natale chiedendo %d regali\n", req.numero_regali);
		int ret = msgsnd(queue_req, &req, sizeof(MessaggioRichiesta) - sizeof(long), 0);
		if (ret < 0) {
			perror("errore nell'invio della letterina\n");
			exit(-3);
		}

		/*
        	TODO: attesa del messaggio di risposta
    	*/

		ret = msgrcv(queue_res, &res, sizeof(MessaggioRisposta) - sizeof(long), getpid(), 0);
		if (ret < 0) {
			perror("non ho ricevuto il riscontro da babbo natale\n");
			exit(-4);
		}
		printf("Ho ricevuto la conferma di ricezione!\n");
		sleep(1);
	}
}

int main() {
	int pid;
	/*
        TODO: recupero code di messaggi
    */
	key_t key1 = ftok("./main.c", 'a');
    key_t key2 = ftok("./main.c", 'b');
	// METTI IL CONTROLLO CON EXCL

    int queue_req = msgget(key1, 0);

    int queue_res = msgget(key2, 0);

	int i;
	for (int i = 0; i < NUM_BAMBINI; i++) {
		/*
        	TODO: completare creazione figli 
    	*/		
		pid = fork();
		if (pid == 0) {
			srand(time(NULL)*getpid());
			printf("Bambino %d sta scrivendo la letterina...\n",getpid());
			bambino(queue_req, queue_res);
		}
			
		
	}

	/*
        TODO: attesa terminazione figli
    */
	for (int i = 0; i < NUM_BAMBINI; i++) {
		wait(NULL);
	}

	msgctl(queue_req,IPC_RMID,0);
	msgctl(queue_res,IPC_RMID,0);

	return 0;
}
