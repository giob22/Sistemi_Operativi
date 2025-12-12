#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "header_msg.h"

int main()
{

    /* Disabilita il buffering su stdout e stderr per scritture immediate */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    /* TBD: Creare le code di messaggi per la send sincrona*/

    key_t kq1 = ftok(".start.c", 'a');
    key_t kq2 = ftok(".start.c", 'b');
    key_t kq3 = ftok("./start.c", 'c');

    // creo le tre code di messaggi

    int request = msgget(kq1, IPC_CREAT | 0664);
    int ok_to_send = msgget(kq2, IPC_CREAT | 0664);
    int msqid = msgget(kq3, IPC_CREAT | 0664);

    pid_t pid;

    /* TBD: Creare un processo figlio, ed eseguire il programma "server" */
    pid = fork();
    if (pid == 0)
    {
        execl("./server", "server", NULL);
        perror("Errore della exec\n");
        exit(-1);
    }

    /* TBD: Creare un processo figlio, ed eseguire il programma "client" */
    pid = fork();
    if (pid == 0)
    {
        execl("./client", "client", NULL);
        perror("Errore della exec\n");
        exit(-1);
    }

    /* TBD: Attendere la terminazione dei processi figli */
    wait(NULL);
    wait(NULL);

    /* TBD: Eliminare le code di messaggi */

    msgctl(request, IPC_RMID, NULL);
    msgctl(msqid, IPC_RMID, NULL);
    msgctl(ok_to_send, IPC_RMID, NULL);
}