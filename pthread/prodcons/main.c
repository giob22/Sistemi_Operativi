#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

#define NUM_THREADS 2 

int main(){

    // creo l'array si pthread_t da creare
    pthread_t threads[NUM_THREADS];

    


    // creo la struttura condivisa che sarà allocata nello heap
    struct prodcons* p = (struct prodcons*) malloc(sizeof(struct prodcons));

    // inizializzo il contenuto della struct
    // riguardante le variabili di sincronizzazione
    // condition variables
    // mutex
    p->stato = VUOTO;
    pthread_mutex_init(&(p->mutex), NULL);
    pthread_cond_init(&(p->ok_produci_cond), NULL);
    pthread_cond_init(&(p->ok_consuma_cond), NULL);
    
    // creo i threads che saranno produttori e consumatori (equal numero altrimenti uno dei due potrebbe non terminare mai)

    // ogni thread è per default joinable, altrimenti dovrei fare una cosa del genere:
    
    pthread_attr_t attributi;
    // inizializzo gli attributi
    pthread_attr_init(&attributi);
    // inserisco gli attributi che desidero un thread
    pthread_attr_setdetachstate(&attributi, PTHREAD_CREATE_JOINABLE); // legato al detach del thread

    for (int i = 0; i < NUM_THREADS; i++){
    	if (i%2 == 0){ // produttore
	    srand(time(NULL) * getpid());
    	    pthread_create(&threads[i], &attributi /*andrebbero gli attributi*/, produttore, (void*)p);
	}else{ // consumatore
	    pthread_create(&threads[i], &attributi, consumatore, (void*)p);
	}
    }

    for (int i = 0; i < NUM_THREADS; i++){
    	pthread_join(threads[i], NULL);
    }

    // ovviamente prima della terminazione devo distruggere mutex e le condition variables create
    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->ok_produci_cond));
    pthread_cond_destroy(&(p->ok_consuma_cond));
    pthread_attr_destroy(&attributi);

    // free della struttura che si trova nello heap
    free(p);


    return 0;
}
