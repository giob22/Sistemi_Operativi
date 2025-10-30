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
    //SEZIONE CRITICA
    
    // gestisco la competizione tra prod
    wait_sem(ds_sem, MUTEX_P);
    // la sezione critica consiste nell'accesso e nell'individuazione del buffer da utilizzare
    // sappiamo che se un produttore entra in sezione critica sará presente sicuramente
    // un valore vuoto all'interno del buffer di stato
    int i = 0;
    while (p->stato[i] != VUOTO)
    {
        i++;
    }
    // nel momento in cui il produttore ha trovato il buffer VUOTO lo setta come IN_USO
    p->stato[i] = IN_USO;
    // finisce la sezione critica poiché a questo punto nessun altro processo puó accedere al buffer 
    // su cui sta lavorando questo produttore
    signal_sem(ds_sem, MUTEX_P);
    // possiamo operare sul buffer senza preoccuparci di RACE CONDITION
    
    p->buffer[i] = rand() % 1000;
    printf("[PID: %d] PRODUTTORE produce: \t%d\n", getpid(), p->buffer[i]);
    // alla fine della produzione il valore del buffer di stato corrispondente 
    // al buffer su cui ha operato il produttore va settato come OCCUPATO
    p->stato[i] = OCCUPATO;
    
    signal_sem(ds_sem, MESS_DISP);
    
}


void consumatore(struct pool_buffer* p,int ds_sem){
    
    // gestisco la cooperazione tra cons e prod
    wait_sem(ds_sem, MESS_DISP);
    // SEZIONE CRITICA

    // gestisco la competizione tra cons
    wait_sem(ds_sem, MUTEX_C);
    // la competizione riguarda l'acesso al buffer di stato e in particolare 
    // alla ricerca del primo elemento OCCUPATO in cui sará presente il dato prodotto.
    // se un consumatore si trova nella sezione critica significa che sicuramente un 
    // buffer é OCCUPATO
    int i = 0;
    while (p->stato[i] != OCCUPATO)
    {
        i++;
    }
    // una volta trovato il buffer il corrispettivo elemento nell'array di stato viene settato
    // in IN_USO cosí che nessun altro processo concorrente possa accedervi causado race condition
    p->stato[i] = IN_USO;
    signal_sem(ds_sem, MUTEX_C);
    // una volta gestita la competizione il processo puó operare tranquillamente sul buffer che ha selezionato


    printf("[PID: %d] CONSUMATORE consuma: \t%d\n", getpid(), p->buffer[i]);
    // una volta finite le operazioni del consumatore questo setta il valore corrispondente al buffer utilizzado il
    // valore VUOTO nel buffer di stato
    p->stato[i] = VUOTO;

    signal_sem(ds_sem, SPAZIO_DISP);


}
