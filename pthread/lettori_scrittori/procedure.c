#include "header.h"




void* scrittore(void* p){
	printf("[S] attivato\n");
	for (int i = 0; i < RIPETI; i++){
		scrivi((Buffer*)p);
	}

	pthread_exit(NULL);
}
void* lettore(void* p){
	printf("[L] attivato\n");
	for (int i = 0; i < RIPETI; i++){
		leggi((Buffer*)p);
	}

	pthread_exit(NULL);
}



void inizio_scrittura(Buffer* p){

	// lo scrittore entra all'interno del monitor
	pthread_mutex_lock(&(p->mutex));

	// nel monitor verifica se sono presenti scrittori o lettori
	// sto utilizzato un monitor con un approccio singal_and_continue
	while (p->num_lettori > 0 || p->num_scrittori > 0){
		// il thread deve sospendersi
		// prima di sospendersi incrementa il numero di scrittori che si trovano in attesa sulla CV
		p->num_scrittori_s++;
		pthread_cond_wait( &(p->ok_scritt), &(p->mutex));
		// nel momento in cui viene sbloccato decrementa il numero di scrittori in attesa
		p->num_scrittori_s--;
	}

	// nel momento in cui viene attivato modifica il numero di scrittori che stanno operando sulla risorsa condivisa

	p->num_scrittori++;


	pthread_mutex_unlock(&(p->mutex));
}


void fine_scrittura(Buffer* p){
	// entra all'interno del monitor
	pthread_mutex_lock(&(p->mutex));

	// decrementa il numero di thread scrittori che stanno operando sulla risorsa
	p->num_scrittori--;

	// verifica su quale categoria di thread eseguire un signal

	if (p->num_scrittori_s > 0)
		pthread_cond_signal(&(p->ok_scritt));
	else
		pthread_cond_signal(&(p->ok_lett)); 
	// infine rilascia il monitor in modo che altri thread possano entrarci

	pthread_mutex_unlock(&(p->mutex));
}

void scrivi(Buffer* p){

	inizio_scrittura(p);

	// inserisco un numero casuale da 0-100
	int valore_prodotto = rand()%100;

	p->buffer = valore_prodotto;

	printf("[S] valore prodotto (%d)\n", p->buffer);

	fine_scrittura(p);

}

void inizio_lettura(Buffer* p){
	// entra nel monitor
	pthread_mutex_lock(&(p->mutex));
	

	// verifica la condizione di sincronizzazione
	while (p->num_scrittori > 0){
		pthread_cond_wait(&(p->ok_lett), &(p->mutex));
	}

	// incrementa il numero di lettori
	p->num_lettori++;

	// esce dal monitor per operare sulla risorsa
	pthread_mutex_unlock(&(p->mutex));
}

void fine_lettura(Buffer* p){
	// entra nel monitor
	pthread_mutex_lock(&(p->mutex));

	// decrementa il numero di lettori
	p->num_lettori--;

	// controlla che non sia l'ultimo lettore → quindi deve segnalare uno scrittore

	if (p->num_lettori == 0)
		pthread_cond_signal(&(p->ok_scritt));

	// esce dal monitor
	pthread_mutex_unlock(&(p->mutex));

}
int leggi(Buffer* p){
	
	inizio_lettura(p);
	// avvio lettura...
	
	int res = p->buffer;

	printf("[L] valore letto: %d\n", p->buffer);
	

	fine_lettura(p);

	return res;

}	




















