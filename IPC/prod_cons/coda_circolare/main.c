#include "semafori.h"
#include "procedure.h"

// gestione semafori
// gestione shm
// gestione lifecycle dei processi

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

#include <stdio.h>

int main()
{

    key_t shm_key = IPC_PRIVATE;

    printf("sizeof(struct pool_buffer): %ld\n", sizeof(struct pool_buffer));

    int ds_shm = shmget(shm_key, sizeof(struct pool_buffer), IPC_CREAT | 0644);

    struct pool_buffer *p = (struct pool_buffer *)shmat(ds_shm, NULL, 0);
    // viene condivisa da tutti i processi che ereditano da questo processo perché sono una copia dell'immagine
    p->coda = 0;
    p->testa = 0;

    for (int i = 0; i < DIM_BUFFER; i++)
    {
        p->buffer[i] = 0;
    }
    key_t sem_key = IPC_PRIVATE; // anche il semaforo é un'area di memoria condivisa
    int ds_sem = semget(ds_sem, 4, IPC_CREAT | 0644);

    // inizializziamo i vari semafori dell'array
    semctl(ds_sem, MESS_DISP, SETVAL, 0);
    semctl(ds_sem, SPAZIO_DISP, SETVAL, DIM_BUFFER);
    semctl(ds_sem, MUTEX_C, SETVAL, 1);
    semctl(ds_sem, MUTEX_P, SETVAL, 1);

    // creo i processi figli che saranno consumatori e produttori
    for (int i = 0; i < NUM_CONSUMATORI; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            printf("[PID: %d] CONSUMATORE avviato \n", getpid());
            sleep(1);
            consumatore(p, ds_sem);
            exit(1); // necessario per evitare una forkbomb
        }
    }
    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {
        pid_t pid = fork();
        srand(getpid() * time(NULL));
        if (pid == 0)
        {
            printf("[PID: %d] PRODUTTORE avviato \n", getpid());
            sleep(1);
            produttore(p, ds_sem);
            exit(1); // necessario per evitare una forkbomb
        }
    }

    // il processo padre aspetta che tutti i processi figli terminino
    for (int i = 0; i < NUM_CONSUMATORI + NUM_PRODUTTORI; i++)
    {
        wait(NULL);
    }
    printf("processo terminato ...\n");

    // marco shm e sem come da eliminare per il kernel
    semctl(ds_sem, 0, IPC_RMID);
    shmctl(ds_shm, IPC_RMID, 0);

    return 0;
}
