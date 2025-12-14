#ifndef _HEADER_SEM_
#define _HEADER_SEM_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define SYNCH 0
#define MUTEXL 1 

typedef struct {

    /* TBD: Completare la struttura con le variabili
            necessarie per lo schema lettori-scrittori
     */

    int buffer;

    int num_lettori;

} lettscritt;


int Wait_Sem(int id_sem, int numsem);
int Signal_Sem(int id_sem, int numsem);

void scrivi(int id_sem, lettscritt * p, int valore);
int leggi(int id_sem, lettscritt * p);

#endif