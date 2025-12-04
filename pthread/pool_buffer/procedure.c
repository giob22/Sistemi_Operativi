#include "header.h"
	

void* produttore(void* p){
    struct prodcons* pc = (struct prodcons*)p;
    // ogni produttore produce 4 volte un valore
    printf("produttore avviato\n");
    for (int i = 0; i < NUM_RIPETIZIONI; i++){
        produci(pc);
    }
    
    #ifdef DEBUG
    LOG("fine produzione");
    #endif
    
    
    pthread_exit(NULL);
}
void* consumatore(void* p){
    struct prodcons* pc = (struct prodcons*)p;
    printf("consumatore avviato\n");
    for (int i = 0; i < NUM_RIPETIZIONI; i++){
        consuma(pc);
    }
    #ifdef DEBUG
    LOG("fine consumazione");
    #endif
    pthread_exit(NULL);    
}




void produci(struct prodcons* pc){
	// prima di iniziare la produzione il produttore deve entrare all'interno del monitor
	pthread_mutex_lock(&(pc->mutex));
    #ifdef DEBUG
    	LOG("produttore è entrato nel mutex");
    #endif
	// sono entrato all'interno del mutex
	
	// verifico la condizione di sincronizzazione per i produttori
	while (pc->num_liberi == 0){
		// devo sospendere il produttore se non sono presenti posti in cui poter produrre
    	#ifdef DEBUG
   			LOG("il produttore si sospende perché non ci sono posti liberi nel buffer");
    	#endif
		pthread_cond_wait(&(pc->ok_produci_cond), &(pc->mutex));
    	#ifdef DEBUG
			if (pc->num_liberi == 0)
    			LOG("il produttore è stato sbloccato ma la condizione di sincronizzazione non è valida, quindi si risospende!!");
			else
				LOG("il produttore è stato sbloccato e la condizione di sincronizzazione è VALIDA, quindi continua la sua produzione");
    	#endif
	}

	// nel momento in cui viene riattivato cerca quale sia il prima buffer VUOTO in cui può inserire il valore creato
	
	int i = 0;
	while (i < MAX_DIM && pc->stato[i] != VUOTO){i++;}
	
    #ifdef DEBUG
    	LOG("buffer trovato (%d)", i);
    #endif
	// nel momento in cui trova il buffer vuoto lo setta come IN USO
	pc->stato[i] = USO;
	// decremento il numero di buffer vouti
	pc->num_liberi--;
	// a questo punto esce dal monitor per permettere l'accesso ad altri threads produttori o consumatori
	// ad accedervi per poter operare sul buffer di stato

	
	pthread_mutex_unlock(&(pc->mutex));
    #ifdef DEBUG
    	LOG("il produttore dopo la produzione rientra nel mutex per modificare le variabili di stato");
    #endif


	// inizia la produzione...


	int valore = rand()%100;

	// inserisco il valore all'interno del buffer
	pc->buffer[i] = valore;
	printf("[P] valore prodotto: %d\n", valore);
	
	// a questo punto devo notificare agli altri threads del valore che ho prodotto e risettare lo stato del buffer a PIENO
	// per farlo il produttore deve rientrare all'interno del monito
	pthread_mutex_lock(&(pc->mutex));

	// incremento il numero di buffer occupati
	pc->num_occupati++;
	// setto lo stato del buffer a PIENO
	pc->stato[i] = PIENO;

	// segnalo un eventuale consumatore bloccato sulla loro condition varables in attesa di un valore da consumare

    #ifdef DEBUG
    	LOG("il produttore segnala sulla CV ok_consuma_cond");
    #endif
	pthread_cond_signal(&(pc->ok_consuma_cond));

	// a questo punto il produttore piò uscire dal monitor
	
    #ifdef DEBUG
    	LOG("il produttore esce dal monitor");
    #endif
	pthread_mutex_unlock(&(pc->mutex));
}
int consuma(struct prodcons* pc){

	// prima di iniziare la consumazione il produttore deve entrare all'interno del monitor
	pthread_mutex_lock(&(pc->mutex));

    #ifdef DEBUG
    	LOG("il consumatore entra nel monitor");
    #endif
	// sono entrato all'interno del mutex
	
	// verifico la condizione di sincronizzazione per i consumatori
	while (pc->num_occupati == 0){
		// devo sospendere il produttore se non sono presenti posti in cui poter consumare
    	#ifdef DEBUG
    		LOG("il consumatore si sospende perché non ci sono buffer PIENI");
   	 	#endif
		pthread_cond_wait(&(pc->ok_consuma_cond), &(pc->mutex));
		
    	#ifdef DEBUG
			if (pc->num_occupati == 0)
    			LOG("il consumatore viene sbloccato ma torna a ribloccarsi sulla CV, perché la condizione si sincronizzazione è NON VALIDA");
			else
				LOG("il consumatore viene sbloccato è la condizione di sincronizzazione è VALIDA");
    	#endif
	}

	// nel momento in cui viene riattivato cerca quale sia il prima buffer PIENO in cui consumera il valore contenuto	
	int i = 0;
	while (i < MAX_DIM && pc->stato[i] != PIENO){i++;}

    #ifdef DEBUG
    	LOG("buffer trovato (%d)", i);
    #endif
	// nel momento in cui trova il buffer vuoto lo setta come IN USO
	pc->stato[i] = USO;
	// decremento il numero di buffer occupati
	pc->num_occupati--;
	// a questo punto esce dal monitor per permettere l'accesso ad altri threads produttori o consumatori
	// ad accedervi per poter operare sul buffer di stato


	pthread_mutex_unlock(&(pc->mutex));


	// inizia la consumazione...
	printf("[C] valore consumato: %d\n", pc->buffer[i]);
	



	// a questo punto devo notificare agli altri threads che il thread ha consumato un valore e risettare lo stato del buffer a VUOTO
	// per farlo il consumatore deve rientrare all'interno del monitor
	pthread_mutex_lock(&(pc->mutex));

    #ifdef DEBUG
    	LOG("il consumatore dopo la consumazione entra nel monitor per modificare le variabili di stato");
    #endif
	// incremento il numero di buffer vuoti
	pc->num_liberi++;
	// setto lo stato del buffer a PIENO
	pc->stato[i] = VUOTO;

	// segnalo un eventuale produttore bloccato sulla loro condition varables in attesa di un valore da produrre

    #ifdef DEBUG
    	LOG("il consumatore risveglia eventuali produttore in attesa che si liberasse un buffer");
    #endif
	pthread_cond_signal(&(pc->ok_produci_cond));
	
	// il consumatore infine esce dal monitor perché ha finito di operare nella sua sezione critica
	pthread_mutex_unlock(&(pc->mutex));

}
