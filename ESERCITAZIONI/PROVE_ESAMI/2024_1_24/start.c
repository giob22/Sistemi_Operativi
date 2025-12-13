#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"
#include "header_sem.h"

int main() {

	
    /* TBD: Creare una coppia di code di messaggi UNIX */
    key_t key_send = ftok("./start.c", 'a');
    key_t key_rcv = ftok("./start.c", 'b');
    int send_queue=msgget(key_send, IPC_CREAT| 0664);
    int rcv_queue=msgget(key_rcv, IPC_CREAT | 0664);

    	printf("[MAIN] %d    %d\n", send_queue,rcv_queue);

    
    
    if(send_queue<0){
    	perror("Errore nella creazione della coda sender\n");
    	exit(1);
    }
	
	if(rcv_queue<0){
    	perror("Errore nella creazione della coda receiver\n");
    	exit(1);
    }
	

    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */

    pid_t pid = fork();
    if (pid == 0) {
        execl("./server", "server", NULL);
        perror("Errore nella sostituzione del codice per il processo server");
        exit(-1);
    }

    /* TBD: Creare 2 processi figli, che eseguano l'eseguibile "client" */
    for (int i = 0; i < NUM_CLIENT; i++) {
        pid = fork();
        if(pid == 0){
            execl("./client", "client", NULL);
            perror("Errore nella sostituzione del codice per il processo client");
            exit(-1);
        }
    }

	
	/* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    
    for (int i = 0; i < NUM_CLIENT + 1; i++) {
        wait(NULL);
    }

    msgctl(send_queue, IPC_RMID, NULL);
    msgctl(rcv_queue, IPC_RMID, NULL);

    return 0;
}
