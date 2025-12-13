#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

int main() {

    /* TBD: Creare le code di messaggi */

    key_t k_queue_req_1 = ftok("./start.c", 'a'); 
    key_t k_queue_res_1 = ftok("./start.c", 'b');
    key_t k_queue_req_2 = ftok("./start.c", 'c');
    key_t k_queue_res_2 = ftok("./start.c", 'd');

    int msqid_req_1 = msgget(k_queue_req_1, IPC_CREAT | 0664); 
    int msqid_res_1 = msgget(k_queue_res_1, IPC_CREAT | 0664);
    int msqid_req_2 = msgget(k_queue_req_2, IPC_CREAT | 0664);
    int msqid_res_2 = msgget(k_queue_res_2, IPC_CREAT | 0664);



    pid_t pid;
    for(int i=0; i<PROCESSI_CLIENT; i++) {

        pid = fork();
        if (pid == 0) {    
            execl("./client", "client", NULL);
            perror("Errore nella sostituzione di codice del client");
            exit(-1);
        }
        /* TBD: Creare i processi client */
    }


    for(int i=0; i<PROCESSI_SERVER; i++) {
        
        /* TBD: Creare i processi server */
        pid = fork();
        if (pid == 0) {
            execl("./server", "server", NULL);
            perror("Errore nella sostituzione di codice del server");
            exit(-1);
        
        }
    }


    /* TBD: Creare il processo database */
    pid = fork();
    if (pid == 0) {
        execl("./database", "database", NULL);
        perror("Errore nella sostituzione di codice del database");
        exit(-1);
    }


    for (int i = 0; i < PROCESSI_CLIENT + PROCESSI_SERVER + 1; i++) {
        wait(NULL);
    }
    /* TBD: Attendere e gestire la terminazione */

    msgctl(msqid_req_1,IPC_RMID, NULL); 
    msgctl(msqid_res_1,IPC_RMID, NULL); 
    msgctl(msqid_req_2,IPC_RMID, NULL); 
    msgctl(msqid_res_2,IPC_RMID, NULL);

    return 0; 

}