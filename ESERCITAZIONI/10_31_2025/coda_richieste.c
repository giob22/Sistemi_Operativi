#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>

#include "coda_richieste.h"

coda_richieste *inizializza_coda()
{
    int shm_id;
    int sem_id;
    coda_richieste *c;

    // Creo un array semaforico di 4 elementi
    key_t k_sem = IPC_PRIVATE;
    sem_id = semget(k_sem, 4, IPC_CREAT | 0664);

    // Semafori per la gestione della cooperazione
    semctl(sem_id, SPAZIO_DISP, SETVAL, DIM);
    semctl(sem_id, MESS_DISP, SETVAL, 0);

    // Semafori per la gestione della competizioni tra prod e cons
    semctl(sem_id, MUTEX_U, SETVAL, 1);
    semctl(sem_id, MUTEX_S, SETVAL, 1);

    // Creo shared memory per prod/cons
    key_t k_shm = IPC_PRIVATE;
    shm_id = shmget(k_shm, sizeof(coda_richieste), IPC_CREAT | 0644);

    if (shm_id < 0)
    {
        perror("Errore creazione SHM coda richieste");
        exit(1);
    }

    // Ottengo un puntatore alla shm
    c = shmat(shm_id, NULL, 0);

    if (c == (void *)-1)
    {
        perror("Errore attach SHM");
        exit(1);
    }

    c->testa = 0;
    c->coda = 0;
    c->sem_id = sem_id;
    c->shm_id = shm_id;

    return c;
}

// Agisco da consumatore
void preleva_richiesta(coda_richieste *c, richiesta *r) {

    /* TODO: Inizio gestione della concorrenza */
    Wait_Sem(c->sem_id,MESS_DISP);

    Wait_Sem(c->sem_id, MUTEX_S); 
    // non dovrebbe servire perché lo shedulatore non compete
    // con nessun altro processo nel consumare i dati contenuti nel buffer.
    // Quindi posso evitare di gestire la competizione in questo caso (consumatore)
    printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

    // Consumazione
    r->posizione = c->vettore[c->coda].posizione;
    r->processo = c->vettore[c->coda].processo;
    c->coda = (c->coda + 1) % DIM;

    /* TODO: Terminazione gestione della concorrenza */

    Signal_Sem(c->sem_id, MUTEX_S);
    Signal_Sem(c->sem_id, SPAZIO_DISP);
}

// Agisco da produttore
void inserisci_richiesta(coda_richieste *c, richiesta *r)
{
    /* TODO: Inizio gestione della concorrenza */
    Wait_Sem(c->sem_id, SPAZIO_DISP);

    Wait_Sem(c->sem_id, MUTEX_U);

    printf("[%d] Produzione in testa: %d\n", getpid(), c->testa);

    // Produzione
    c->vettore[c->testa].posizione = r->posizione;
    c->vettore[c->testa].processo = r->processo;
    c->testa = (c->testa + 1) % DIM;

    /* TODO: Terminazione gestione della concorrenza */
    Signal_Sem(c->sem_id, MUTEX_U);
    Signal_Sem(c->sem_id, MESS_DISP);
}

void rimuovi_coda(coda_richieste *c){

    /* TODO: Marcare come da eliminare shm e sem */

    semctl(c->sem_id,0,IPC_RMID);
    shmctl(c->shm_id, IPC_RMID, 0);

}
