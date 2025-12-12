#ifndef _HEADER_PRODCONS_H_
#define _HEADER_PRODCONS_H_

#include <sys/sem.h>

// ho bisogno di 4 semafori per gestire la cooperazione e competizione tra i processi
#define SPAZIO 0
#define MESS 1
#define MUTEXP 2
#define MUTEXC 3

#define DIM_BUFF 2

typedef struct {

    int valore[DIM_BUFF];
    //TODO: aggiungere variabili per gestione circolare con semafori
    int testa,coda;


    int semid;



} BufferProdCons;

#endif