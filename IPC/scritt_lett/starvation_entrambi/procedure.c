#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "procedure.h"
#include "semafori.h"



void Inizio_Scrittura(int sem, Buffer* buff){
    
    Wait_sem(sem, MUTEXS);
    
    buff->num_scrittori++;
    if (buff->num_scrittori == 1) // il processo è il primo scrittore quindi esegue una wait su SYNCH
        Wait_sem(sem, SYNCH);
    Signal_sem(sem, MUTEXS); // libero la risorsa num_scrittori per permettere agli altri processi di potervi accedere
    // come ultima operazione per uno scrittore nell'iniziare a scrivere è di fare una wait su MUTEX
    // perché la scrittura deve essere un operazione eseguita in mutua esclusione
    Wait_sem(sem, MUTEX);    
}
void Fine_Scrittura(int sem, Buffer* buff){
    Signal_sem(sem, MUTEX); // rilascio della shm per permette agli altri scrittori di scrivere se sono in attesa
    // penso che si faccia prima per permettere subito ad altri processi scrittori di operare sulla shm
    // mentre il processo corrente aggiorna il numero di processi scrittori
    
    // si aggiorna il num_scrittori
    Wait_sem(sem, MUTEXS);
    buff->num_scrittori--;
    if(buff->num_scrittori == 0) // significa che il processo è l'ultimo scrittore -> deve segnalare che la risorsa è libera per eventuali lettori, o altri scrittori successivi
    Signal_sem(sem, SYNCH);
    // rilascio della variabile num_scrittore
    Signal_sem(sem, MUTEXS);
}
// l'implementazione per inizio e fine lettura è identica al caso di scrittori/lettori con starvation degli scrittori.
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


void Lettore(int sem, Buffer* buff){
    Inizio_Lettura(sem,buff);

    sleep(1);
    printf("[SCRITTORE: %d] valore letto: %ld\n", getpid(), buff->messaggio);

    Fine_Lettura(sem, buff);
}
void Scrittore(int sem, Buffer* buff){
    Inizio_Scrittura(sem, buff);
    
    struct timeval t1;
    gettimeofday(&t1, NULL);
    buff->messaggio = t1.tv_usec;
    sleep(1);
    printf("[SCRITTORE: %d] valore scritto: %ld\n", getpid(), buff->messaggio);

    Fine_Scrittura(sem,buff);
}