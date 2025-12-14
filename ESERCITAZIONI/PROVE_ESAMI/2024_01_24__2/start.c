#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {


    /* TBD: Predisporre le code di messaggi UNIX per la send sincrona */
    key_t key_ots = ftok("./start.c", 'o');
    key_t key_rts = ftok("./start.c", 'r');
    key_t key_m = ftok("./start.c", 'm');

    int queue_rts = msgget(key_rts, IPC_CREAT | 0664);
    int queue_ots = msgget(key_ots, IPC_CREAT | 0664); 
    int queue_msg = msgget(key_m, IPC_CREAT | 0664);


    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */
    pid_t pid = fork();
    if (pid == 0) {
        execl("./server", "server", NULL);
        perror("Errore exec");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        execl("./client", "client", NULL);
        perror("Errore exec");
        exit(2);
    }
    /* TBD: Creare un processo figlio, che esegua l'eseguibile "client" */


    /* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    msgctl(queue_msg, IPC_RMID, NULL);
    msgctl(queue_rts, IPC_RMID, NULL);
    msgctl(queue_ots, IPC_RMID, NULL);

    printf("APPLICAZIONE TERMINATA....\n");
    return 0;
}