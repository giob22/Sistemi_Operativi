#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "procedure.h"

int main(){

    int sem_id,shm_id, numlettori, numscrittori;
    key_t k_sem = IPC_PRIVATE;
    key_t k_shm = IPC_PRIVATE;

    pid_t pid;

    Buffer* buff;

    numlettori = 6;
    numscrittori = 6;
    // il numero di scrittori e lettori non deve essere necessariamente lo stesso come lo era per il problema consumatori/produttori
    int numprocessi = numlettori + numlettori;
     
    shm_id = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0644);

    buff = (Buffer*) shmat(shm_id, NULL, 0);
    buff->num_lettori = 0;
    buff->msg = 0;

    //creiamo i due semafori
    sem_id = semget(k_sem, 2, IPC_CREAT | 0644);
    semctl(sem_id, MUTEXL, SETVAL, 1);
    semctl(sem_id, SYNCH, SETVAL, 1);


    //generiamo scrittori e lettori
    for (int k = 0; k < numprocessi; k++)
    {
        pid = fork();
        if (pid == 0)
        {
            if ((k%2) == 0)
            {
                printf("[SCRITTORE: %d]\n", getpid());
                Scrittore(sem_id, buff);
            }else {
                printf("[LETTORE: %d]\n",getpid());
                Lettore(sem_id, buff);
            }
            exit(0);
        }
        // TODO: inserisci anche la verifica dello stato di terminazione di ogni figlio
        for (int k = 0; k < numprocessi; k++)
        {
            wait(NULL);
        }

        // marchiamo come eliminabili i semafori e la shm
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID);
        
        
    }

    return 0;
}