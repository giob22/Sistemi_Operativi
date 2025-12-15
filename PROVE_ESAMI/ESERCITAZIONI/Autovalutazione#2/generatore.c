#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "header.h"

void text_casuale(char text[], int dim){
    for (int i = 0; i < dim - 1; i++)
    {
        text[i] = 'a' + rand()%26;
    }
    text[dim - 1] = '\0';
}
void interi_casuale(int interi[], int dim){
    for (int i = 0; i < dim; i++)
        interi[i] = rand()%10;   
}


void generatore_produttore(struct ProdConsGen *pc){
        
    /* TODO: utilizzare il costrutto monitor per la produzione del messaggio */

    enter_monitor(&(pc->m));

    // verifico la condizione di sincronizzazione
    // sto utilizzando un monitor signal-and-continue quindi questa condizione non è scontato
    // che sia ancora verificata quando il processo che si era sospeso per questa ritorna ad eseguire
    // quindi ogni qual volta che il processo torna ad eseguire deve controllare la condizione
    // → utilizzo un while

    // un produttore potrà produrre solo solo se è presente almeno un buffer libero all'interno della coda
    while (pc->dim == DIM_QUEUE) // significa che la coda di buffer è piena, quindi non si può produrre
        wait_condition(&(pc->m), VARCOND_GEN_PRODUTTORI);
    
    
    // inizia la produzione del messaggio
    text_casuale(pc->msgs[pc->testa].text, STRING_MAX_DIM);
    interi_casuale(pc->msgs[pc->testa].interi, INT_MAX_DIM);
    pc->msgs[pc->testa].var = 0;
    pc->msgs[pc->testa].type = MSG_TYPE;

    printf("[generatore_produttore] Messaggio generato!\n");
    printf("[generatore_produttore] ...............msg.stringa: %s\n", pc->msgs[pc->testa].text /* TODO */);
    printf("[generatore_produttore] ...............msg.array[0]: %d\n", pc->msgs[pc->testa].interi[0] /* TODO */);
    printf("[generatore_produttore] ...............msg.array[1]: %d\n", pc->msgs[pc->testa].interi[1] /* TODO */);
    printf("[generatore_produttore] ...............msg.var: %d\n", pc->msgs[pc->testa].var /* TODO */);

    /* TODO: ... */
    // aggiorno le variabili di stato della shm
    pc->testa = (pc->testa + 1)% DIM_QUEUE;
    (pc->dim)++;
    // segnalo un eventuale consumatore in attesa sulla condition variable dedicata a tale categoria di processo
    signal_condition(&(pc->m), VARCOND_GEN_CONSUMATORI);
    leave_monitor(&(pc->m));
}

void generatore_consumatore(struct ProdConsGen *pc, int ds_queue_gen_filter){

    /* TODO: utilizzare il costrutto monitor per la consumazione del messaggio e l'invio verso il processo filter */

    enter_monitor(&(pc->m));
    // verifico la condizione di sincronizzazione per i consumatori
    // → se valida allora significa che il buffer è vuoto e il consumatore deve attendere sulla condition variable
    // dedicata ai consumatori
    // utilizzo sempre un while perché stiamo usando un monitor signal-and-continue

    // se la coda è vuota → il processo consumatore deve sospendersi su VARCON_GEN_CONSUMATORI
    while (pc->dim == 0)
        wait_condition(&(pc->m), VARCOND_GEN_CONSUMATORI);
    
    printf("[generatore_consumatore] Messaggio CONSUMATO!\n");
    printf("[generatore_consumatore] ...............msg.stringa: %s\n", pc->msgs[pc->coda].text);
    printf("[generatore_consumatore] ...............msg.array[0]: %d\n", pc->msgs[pc->coda].interi[0]);
    printf("[generatore_consumatore] ...............msg.array[1]: %d\n", pc->msgs[pc->coda].interi[1]);
    printf("[generatore_consumatore] ...............msg.var: %d\n", pc->msgs[pc->coda].var);

    // invio il messaggio in coda sulla message queue gen_filter
    
    /* TODO: invio del messaggio consumato al processo filter */
    int ret = msgsnd(ds_queue_gen_filter, &(pc->msgs[pc->coda]), sizeof(message) - sizeof(long), 0); // send asincrona bloccante nel caso di coda dei messaggi piena

    if (ret<0){
            perror("msgsnd del messaggio on ds_queue_gen_filter FALLITA!");
            exit(-1);
    }
    printf("[generatore_consumatore] Messaggio INVIATO!\n");
    
    /* TODO: ... */
    // modifico le variabili di stato della coda circolare
    pc->coda = (pc->coda + 1)%DIM_QUEUE;
    (pc->dim)--;

    // il consumatore segnala un eventuale produttore in attesa
    signal_condition(&(pc->m), VARCOND_GEN_PRODUTTORI);
    leave_monitor(&(pc->m));
}

