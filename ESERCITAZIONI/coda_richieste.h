#ifndef _CODA_RICHIESTE_H_
#define _CODA_RICHIESTE_H_

#include "semafori.h"

#define DIM 10

/* TODO: definire le eventuali macro per indicizzare i semafori */
#define SPAZIO_DISP 0
#define MESS_DISP 1

#define MUTEX_U 2
#define MUTEX_S 3
// CONSIDERAZIONI:
// non dovrebbe servire perché lo shedulatore non compete
// con nessun altro processo nel consumare i dati contenuti nel buffer.
// Quindi posso evitare di gestire la competizione in questo caso (consumatore), 
// non ho problemi di race condition

typedef struct {
    unsigned int posizione;
    pid_t processo;
} richiesta;

typedef struct {
    int sem_id;
    int shm_id;
    
 
    /* TODO: aggiungere pool di buffer e variabili per gestire la produzione e consumazione 
        in coda circolare
    */
    richiesta vettore[DIM];
    int testa,coda;
} coda_richieste;

coda_richieste * inizializza_coda();
void preleva_richiesta(coda_richieste *c, richiesta * r);
void inserisci_richiesta(coda_richieste *c, richiesta * r);
void rimuovi_coda(coda_richieste *c);

#endif
