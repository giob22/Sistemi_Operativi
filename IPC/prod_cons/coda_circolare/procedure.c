#include "semafori.h"
#include "procedure.h"

//gestione semafori
//gestione shm
//gestione lifecycle dei processi


#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>




void produttore(struct pool_buffer* p, int ds_sem){

    // gestisco la cooperazione tra cons e prod
    wait_sem(ds_sem, SPAZIO_DISP);
    
    // gestisco la competizione tra prod
    wait_sem(ds_sem, MUTEX_P);
    //SEZIONE CRITICA
    p->buffer[p->testa] = rand() % 1000;
    printf("[PID: %d] PRODUTTORE produce: \t%d\n", getpid(), p->buffer[p->testa]);
    p->testa = (p->testa + 1)%DIM_BUFFER;
    
    signal_sem(ds_sem, MUTEX_P);
    signal_sem(ds_sem, MESS_DISP);
    
}


void consumatore(struct pool_buffer* p,int ds_sem){
    
    // gestisco la cooperazione tra cons e prod
    wait_sem(ds_sem, MESS_DISP);

    // gestisco la competizione tra cons
    wait_sem(ds_sem, MUTEX_C);

    // SEZIONE CRITICA
    printf("[PID: %d] CONSUMATORE consuma: \t%d\n", getpid(), p->buffer[p->coda]);
    p->coda = (p->coda + 1) % DIM_BUFFER;

    signal_sem(ds_sem, MUTEX_C);
    signal_sem(ds_sem, SPAZIO_DISP);


}
