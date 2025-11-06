#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "procedure.h"

int main(){
    int id_sem, id_shared, k;
        key_t k_shm = IPC_PRIVATE;
        key_t k_sem = IPC_PRIVATE;
        Buffer *buff;

        pid_t pid;
        int num_processi = 10;

        id_shared = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0644);
        buff = (Buffer*) shmat(id_shared, NULL, 0);

        // inizializziamo la struttura dati
        buff->messaggio = 0;
        buff->num_lettori = 0;
        buff->num_scrittori = 0;

        id_sem = semget(k_sem, 4, IPC_CREAT | 0644);

        semctl(id_sem, MUTEX, SETVAL, 1);
        semctl(id_sem, MUTEXL, SETVAL, 1);
        semctl(id_sem, MUTEXS, SETVAL, 1);
        semctl(id_sem, SYNCH, SETVAL, 1);

        for (int k = 0; k < num_processi; k++)
        {
            pid = fork();
            if (pid == 0)
            {
                
                if ((k%2) == 0)
                {
                    printf("sono il figlio scrittore. Il mio pid %d \n", getpid());
                    Scrittore(id_sem, buff);
                }else
                {
                    printf("sono il figlio lettore. Il mio pid %d \n", getpid());
                    Lettore(id_sem, buff);
                }
                exit(0);
            }
            
        }

        for (int k = 0; k < num_processi; k++)
        {
            wait(NULL);
        }

        // marco come eliminabili il semaphor set e shm
        semctl(id_sem, 0, IPC_RMID);
        shmctl(id_shared, IPC_RMID, NULL);

        return 0;

}