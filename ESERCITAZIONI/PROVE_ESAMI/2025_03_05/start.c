#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "header_msg.h"

int main() {

    /* TBD: Creare le code di messaggi per la send sincrona*/
    key_t key_rts = ftok(".", 'a');
    key_t key_ots = ftok(".", 'b');
    key_t key_req = ftok(".", 'c');

    int msqid_rts = msgget(key_rts, IPC_CREAT | 0664);
    int msqid_ots = msgget(key_ots, IPC_CREAT | 0664);
    int msqid_req = msgget(key_req, IPC_CREAT | 0664);
    /* TBD: Creare un processo figlio, ed eseguire il programma "server" */
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execl("./server", "server", NULL);
        perror("errore nella exec\n");
        exit(1);
    }
    
    
    
    /* TBD: Creare un processo figlio, ed eseguire il programma "client" */
    pid = fork();
    if (pid == 0)
    {
        execl("./client", "client", NULL);
        perror("errore nella exec\n");
        exit(1);
    }
    

    /* TBD: Attendere la terminazione dei processi figli */
    for (int  i = 0; i < 2; i++)
    {
        wait(NULL);
    }
    


    /* TBD: Eliminare le code di messaggi */
    msgctl(msqid_rts, IPC_RMID, NULL);
    msgctl(msqid_ots, IPC_RMID, NULL);
    msgctl(msqid_req, IPC_RMID, NULL);   
    printf("FINE APPLICAZIONE....\n"); 
    return 0;


}