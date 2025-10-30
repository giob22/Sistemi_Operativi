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


void produttore(struct pool_buffer*shm_ptr, int ds_sem){
    srand(time(NULL));
    wait_sem(ds_sem, SPAZIO_DISP);
    // sez. critica
    //*shm_ptr = rand() % 100; // single buffer
    // utilizzo una coda circolare 
    wait_sem(ds_sem, MUTEX_PROD);
    //SEZ. CRITICA
    
    
    shm_ptr->buffer[shm_ptr->testa] = rand() % 1000;
    printf("[PID: %d] PRODUTTORE produce %d\n", getpid(),shm_ptr->buffer[shm_ptr->testa]);
    shm_ptr->testa = (shm_ptr->testa + 1) % DIM_BUFFER;
    
    sleep(1);
    signal_sem(ds_sem,MUTEX_PROD);
    signal_sem(ds_sem, MESS_DISP);
}


void consumatore(struct pool_buffer* shm_ptr,int ds_sem){
    wait_sem(ds_sem, MESS_DISP);
    wait_sem(ds_sem, MUTEX_CONS);
    // sez. critica
    
    printf("[PID: %d] CONSUMATORE consuma %d\n", getpid(), shm_ptr->buffer[shm_ptr->coda]);
    shm_ptr->coda = (shm_ptr->coda + 1) % DIM_BUFFER;
    sleep(1);
    
    
    signal_sem(ds_sem, MUTEX_CONS);
    signal_sem(ds_sem, SPAZIO_DISP);
}
