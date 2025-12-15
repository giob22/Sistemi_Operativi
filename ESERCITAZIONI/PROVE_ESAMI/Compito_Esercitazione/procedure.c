#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#include "header.h"
#include "semafori.h"

void inizializza_vettore(VettoreProdCons * p) {

   // inizializza struttura coda circolare
   p->testa = 0;
   p->coda = 0;
   p->dim = 0;
   init_monitor(&(p->m), 2);
}

void produci(VettoreProdCons * p, int valore) {

    // produzione con meccanismo di coda circolare
    enter_monitor(&(p->m));
    // condizione di sincronizzazione
    if (p->dim == DIM) {
        wait_condition(&(p->m), CV_PROD);
    }
    p->dim++;
    p->buffer[p->testa] = valore;
    p->testa = (p->testa + 1)%DIM;
    signal_condition(&(p->m), CV_CONS);

    leave_monitor(&(p->m));
}

int consuma(VettoreProdCons * p) {
    int res;
    // consumo con meccanismo di coda circolare
    enter_monitor(&(p->m));

    // codizione di sincronizzazione
    if (p->dim == 0) {
        wait_condition(&(p->m), CV_CONS);
    }
    p->dim--;
    res = p->buffer[p->coda];
    p->coda = (p->coda + 1)%DIM;

    signal_condition(&(p->m), CV_PROD);
    leave_monitor(&(p->m));
    return res;
}

void rimuovi_vettore(VettoreProdCons * p) {

    // 
    remove_monitor(&(p->m));
    
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

   // inizializzazione semaforo per aggiornare il buffer in mutua esclusione
   p->buffer = 0;
   p->semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0664);
   semctl(p->semid, MUTEX, SETVAL, 1);

}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    Wait_Sem(p->semid, MUTEX);
    // aggiorno valore in mutua esclusione
    p->buffer+=valore;

    Signal_Sem(p->semid, MUTEX);

}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    //
    semctl(p->semid, MUTEX, IPC_RMID);
}


