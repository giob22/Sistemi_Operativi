#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"



int main(){

    // creo l'array si pthread_t da creare
    pthread_t threads[NUM_SCRITTORI + NUM_LETTORI];

    

    // creo la struttura condivisa che sarà allocata nello heap
    Buffer* p = (Buffer*) malloc(sizeof(Buffer));

    // inizializzo il contenuto della struct
    // riguardante le variabili di sincronizzazione
    // condition variables
    // mutex
	p->num_lettori = 0;
	p->num_scrittori = 0;
	p->num_scrittori_s = 0;
	pthread_mutex_init(&(p->mutex), NULL);
	pthread_cond_init(&(p->ok_lett), NULL);
	pthread_cond_init(&(p->ok_scritt), NULL); 
    // creo i threads che saranno produttori e consumatori (equal numero altrimenti uno dei due potrebbe non terminare mai)

    // ogni thread è per default joinable, altrimenti dovrei fare una cosa del genere:

    pthread_attr_t attributi;
    // inizializzo gli attributi
    pthread_attr_init(&attributi);
    // inserisco gli attributi che desidero un thread
    pthread_attr_setdetachstate(&attributi, PTHREAD_CREATE_JOINABLE); // legato al detach del thread


	// creo i pthread
	// scrittori
	for	(int i = 0; i < NUM_SCRITTORI; i++){
		srand(time(NULL) * getpid());
		pthread_create(&threads[i], &attributi, scrittore, (void*)p);	
	}

	for (int i = NUM_SCRITTORI; i < NUM_SCRITTORI + NUM_LETTORI; i++){
		pthread_create(&threads[i], &attributi, lettore, (void*)p);
	}


    for (int i = 0; i < NUM_SCRITTORI + NUM_LETTORI; i++){
    	pthread_join(threads[i], NULL);
    }

    // ovviamente prima della terminazione devo distruggere mutex e le condition variables create
    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->ok_lett));
    pthread_cond_destroy(&(p->ok_scritt));
    pthread_attr_destroy(&attributi);

    // free della struttura che si trova nello heap
    free(p);


    return 0;
}
