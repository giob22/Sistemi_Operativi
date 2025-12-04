#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

#define NUM_THREADS 10 

int main(){

    // creo l'array si pthread_t da creare
    pthread_t threads[NUM_THREADS];

    

    // creo la struttura condivisa che sarà allocata nello heap
    struct prodcons* p = (struct prodcons*) malloc(sizeof(struct prodcons));
    #ifdef DEBUG
        LOG("creo l'area di memoria nello heap per contenere la risorsa condivisa, indirizzo: %p", p);
    #endif

    // inizializzo il contenuto della struct
    // riguardante le variabili di sincronizzazione
    // condition variables
    // mutex
    #ifdef DEBUG
        LOG("inizializzo:\n1) le variabili per la sinconizzazione\n2) il mutex\n3) le condition variables");
    #endif
    

	// inizializzo lo stato a vuoto, e num_liberi a MAX_DIM e num_occupati a 0
	p->num_occupati = 0;
	p->num_liberi = MAX_DIM;
	for (int i = 0; i < MAX_DIM; i++){
		p->stato[i] = VUOTO;
	}

    pthread_mutex_init(&(p->mutex), NULL);
    pthread_cond_init(&(p->ok_produci_cond), NULL);
    pthread_cond_init(&(p->ok_consuma_cond), NULL);
    
    // creo i threads che saranno produttori e consumatori (equal numero altrimenti uno dei due potrebbe non terminare mai)

    // ogni thread è per default joinable, altrimenti dovrei fare una cosa del genere:
    #ifdef DEBUG
        LOG("inizializzo gli attributi per i thread per fare in modo che siano joinable");
    #endif

    pthread_attr_t attributi;
    // inizializzo gli attributi
    pthread_attr_init(&attributi);
    // inserisco gli attributi che desidero un thread
    pthread_attr_setdetachstate(&attributi, PTHREAD_CREATE_JOINABLE); // legato al detach del thread

    for (int i = 0; i < NUM_THREADS; i++){
    	if (i%2 == 0){ // produttore
	        srand(time(NULL) * getpid());
    	    pthread_create(&threads[i], &attributi /*potrei inserire anche NULL perché un thread è joinable per default*/
                           , produttore, (void*)p);
            #ifdef DEBUG
                LOG("thread produttore creato");
            #endif
	}else{ // consumatore
	    pthread_create(&threads[i], &attributi, consumatore, (void*)p);
        #ifdef DEBUG
            LOG("thread consumatore creato");
        #endif
	}
    }

    #ifdef DEBUG
        LOG("attendo la terminazione dei thread creati");
    #endif
    for (int i = 0; i < NUM_THREADS; i++){
    	pthread_join(threads[i], NULL);
    }

    // ovviamente prima della terminazione devo distruggere mutex e le condition variables create
    #ifdef DEBUG
        LOG("fatto il destroy del mutex, condition variables e free dell'area di memoria allocata nello heap");
    #endif
    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->ok_produci_cond));
    pthread_cond_destroy(&(p->ok_consuma_cond));
    pthread_attr_destroy(&attributi);

    // free della struttura che si trova nello heap
    free(p);


    return 0;
}
