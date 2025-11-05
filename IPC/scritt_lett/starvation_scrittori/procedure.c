#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "procedure.h"
#include "../../sem/semafori.h"

void Inizio_Lettura(int sem, Buffer *buff){
    Wait_sem(sem, MUTEXL); // per evitare che i lettori accedino a num_lettori
    buff->num_lettori++; 
    if (buff->num_lettori == 1) 
        Wait_sem(sem, SYNCH); // se è il primo processo allora sospende gli eventuali scrittori che vogliono accedere alla shm 
    Signal_sem(sem, MUTEXL);// rilascia la variabile per far entrare altri lettori
}
void Fine_Lettura(int sem, Buffer* buff){
    Wait_sem(sem, MUTEXL);

    buff->num_lettori--;
    if (buff->num_lettori == 0)
        Signal_sem(sem, SYNCH); // se è verificata la condizione allora questo è l'ultimo processo lettore ad avere accesso alla shm
                                // quindi la libera permettendo a scrittori di scrivere sulla shm
    Signal_sem(sem, MUTEXL); // rilascio del mutexl per permettere l'accesso gli altri lettori
}

void Inizio_Scrittura(int sem){
    Wait_sem(sem, SYNCH);
}
void Fine_Scrittura(int sem){
    Signal_sem(sem, SYNCH);
}


void Scrittore(int sem, Buffer* buff){
    Inizio_Scrittura(sem);

    // ******SCRITTURA******
    struct timeval t1;
    gettimeofday(&t1,NULL);
    buff->msg = t1.tv_usec;
    sleep(1);
    printf("Valore scritto: %ld", buff->msg);

    Fine_Scrittura(sem);
}

void Lettore(int sem, Buffer* buff){
    Inizio_Lettura(sem,buff);

    // ******LETTURA******
    sleep(1);
    printf("Valore letto: %ld\nnumero_lettori: %d\n", buff->msg,buff->num_lettori);

    Fine_Lettura(sem,buff);
}
