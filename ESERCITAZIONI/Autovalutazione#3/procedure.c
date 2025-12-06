#include "header.h"



void inizializza(struct monitor* m){

	m->molo=0;
	m->id_nave=0;

    /* TODO: Inizializzare le variabili dell'algoritmo, il mutex, e le variabili condition */

	// variabili per la gestione della sincronizzazione
	m->num_scrittori = 0;
	m->num_lettori = 0;
	m->num_scrittori_s = 0;

	// mutex per la mutua esclusione, CV per la cooperazione
	pthread_mutex_init(&(m->mutex), NULL);
	pthread_cond_init(&(m->ok_lett), NULL);
	pthread_cond_init(&(m->ok_scritt), NULL);
}

void rimuovi (struct monitor* m){
    /* TODO: Disattivare mutex e variabili condition */

	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_scritt));
	pthread_cond_destroy(&(m->ok_lett));

}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_molo(struct monitor* m, int molo){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della SCRITTURA
     */

	// inizio INIZIO_SCRITTURA
	pthread_mutex_lock(&(m->mutex));

	// verifica la condizione di sincronizzazione
	while (m->num_lettori > 0 || m->num_scrittori > 0){
		m->num_scrittori_s++;
		pthread_cond_wait(&(m->ok_scritt), &(m->mutex));
		m->num_scrittori_s--;
	}
	// incrementa il numero di scrittori
	m->num_scrittori++;

	// esce dal monitor
	pthread_mutex_unlock(&(m->mutex));
	// fine INIZIO_SCRITTURA



	// viene impostato il nuovo molo per la nave

	m->molo = molo;



	// inizio FINE_SCRITTURA
	pthread_mutex_lock(&(m->mutex));

	// decrementa il numero di scrittori
	m->num_scrittori--;
	
	// determina su quale categoria di thread eseguire una signal
	// verifica prima se sono in attesa altri scrittori → Starvation di entrambi
	if (m->num_scrittori_s > 0)
		pthread_cond_signal(&(m->ok_scritt));
	else
		pthread_cond_broadcast(&(m->ok_lett));

	// uscita dal monitor
	pthread_mutex_unlock(&(m->mutex));
	// fine FINE_SCRITTURA
}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_molo(struct monitor* m){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della LETTURA
     */
	
	// inizio INIZIO_LETTURA

	pthread_mutex_lock(&(m->mutex));
	
	// verifica la condizione di sincronizzazione
	// utilizzo una while perché il monitor implementa una politica signal_and_continue
	while (m->num_scrittori > 0){
		pthread_cond_wait(&(m->ok_lett), &(m->mutex));
	}

	// incremento il numero di lettori
	m->num_lettori++;

	// uscita dal monitor
	pthread_mutex_unlock(&(m->mutex));


	// lettura del molo

	int result = m->molo;



	// inizio FINE_LETTURA
	pthread_mutex_lock(&(m->mutex));


	// decrementa il numero di lettori
	m->num_lettori--;

	// il thread lettore verifica che non sia l'ultimo lettore 
	// in tal caso farà una signal per risvegliare un thread scrittore

	if (m->num_lettori == 0)
		pthread_cond_signal(&(m->ok_scritt));
	
	// se non è l'ultimo lettore esce direttamente dal monitor

	pthread_mutex_unlock(&(m->mutex));

	return result;
}



























