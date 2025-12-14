#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "header.h"

int main() {
/*
    TODO: completare la creazione della memoria condivisa e inizializzazione struttura
*/

key_t key_app = IPC_CREAT;

int shmid = shmget(key_app, sizeof(MonitorPC), IPC_CREAT | 0664);

if (shmid < 0) {
    perror("Errore nella creazione della shared memory\n");
    exit(4);
}

MonitorPC* pc = (MonitorPC*) shmat(shmid, NULL, 0);

inizializza_coda(pc);

/*
    TODO: completare la creazione della coda di messaggi
*/
int msqid = msgget(key_app, IPC_CREAT | 0664);
if (msqid < 0) {
    perror("Errore nella creazione della coda di messaggi\n");
}    


/*
    TODO: figlio consumatore
*/


    pid_t pidp;
    for (int i = 0; i < NUM_BAMBINI; i++) {
        /*
            TODO: completare la creazione dei figli produttori
        */
        pidp = fork();
        if (pidp == 0) {
            srand(time(NULL)*getpid());
            Produttore(pc,rand()%10+1,msqid);
            sleep(rand() % 3 + 1);
            exit(0);
        }

    }
    pid_t pidc = fork();
    if (pidc == 0) {
        Consumatore(pc, msqid);
        exit(0);
    }

    /*
        TODO: attesa terminazione processi e rimozione strutture
    */

    for (int i = 0; i < NUM_BAMBINI + 1; i++) {
        wait(NULL);
    }

    msgctl(msqid, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    rimuovi_coda(pc);

    printf("TERMINAZIONE......\n");

    return 0;
}
