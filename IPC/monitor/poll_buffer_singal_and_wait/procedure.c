#include "procedure.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int consuma(Buffer* pc){
	// entriamo dentro il monitor per iniziare la consumazione in mutua esclusione 
	enter_monitor(&(pc->m));

	// ci troviamo all'interno del monitor, dobbiamo verificare se sono presenti dei messaggi da leggere
	if (pc->num_occupati == 0)
	{
		// se la condizione é verificata allora significa che non sono presenti dei buffer da poter leggere -> il processo di mette in attesa
		// su una variabile condition
		printf("[CONSUMATORE] mi metto in attesa sulla condition variable VARCOND_CONSUMATORI\n");
		wait_condition(&(pc->m), VARCOND_CONSUMATORI);
		printf("[CONSUMATORE] sono stato segnalato sulla condition variable VARCOND_CONSUMATORI\n");
	}


	// sezione critica 
	// ricerca del buffer nel pool di buffer che sia occupato da un dato prodotto
	int i = 0;
	while (i < DIM_BUFFER && pc->stato[i] != OCCUPATO)
	{
		i++;
	}
	// una volta trovato la posizione corrispondente ad un buffer occupato da un dato, setto lo stato di questa posizione come USO
	// in modo che nessun altro processo possa accedervi
	// permettendo cosí ad altri processi che siano lettori o scrittori di operare in concorrenza senza pericolo di RACE CONDITION

	pc->stato[i] = USO;
	pc->num_occupati--;
	// finisce la prima parte della sezione critica
	// in questo momento il processo puó accedere in mutua esclusione al buffer i-esimo del pool di buffer
	leave_monitor(&(pc->m));

	// effettua una operazione di lettura sulla posizione i-esima del pool di buffer 
	printf("[CONSUMATORE] valore letto: %d\n", pc->buffer[i]);
	int res = pc->buffer[i];

	// a questo punto devo rientrare nella sezione critica perché devo aggiornare il numero di occupati e il numero di liberi.
	// inoltre devo settare lo stato della posizione i-esima del buffer di stato ad VUOTO poiché il valore é stato CONSUMATO

	enter_monitor(&(pc->m));
	pc->num_liberi++;
	pc->stato[i] = VUOTO;

	// prima di uscire dal monitor devo segnalare i produttori che sono in attesa sulla variabile VARCOND_PRODUTTORI
	signal_condition(&(pc->m), VARCOND_PRODUTTORI);// si sospende se sono presenti dei processi sospesi sulla condition variable
	leave_monitor(&(pc->m));
	
	return res;
}
void produci(Buffer* pc, int valore_prodotto){
	// entro nel monitor per operare sul vettore di stato in modo da ottenere un singolo buffer del pool di buffer in mutua esclusione
	enter_monitor(&(pc->m));
	// una volta entrato nel monitor un processo prduttore deve verificare se sono presenti
	// buffer vuoti su cui produrre dei dati, altrimenti si sospende sulla variabile condition dedicata ai produttori

	if (pc->num_liberi == 0) // significa che tutti i buffer sono occupati o in uso, quindi il processo corrente deve sospendersi 
	{
		printf("[PRODUTTORE] mi sono sospeso sulla condition variable VARCOND_PRODUTTORI\n");
		wait_condition(&(pc->m), VARCOND_PRODUTTORI); 
		printf("[PRODUTTORE] sono stato sbloccatop dalla condition variable VARCOND_PRODUTTORI\n");
	}
	// nel momento in cui la condizione di sincronizzazione é stata varificata si passa alla produzione
	// pprima di arrivare alla produzione dobbiamo identificare il buffer libero é renderlo accessibile unicamente al processo corrente
	int i = 0;
	while (i<DIM_BUFFER && pc->stato[i] != VUOTO)
	{
		i++;
	}
	// a questo punto otteniamo sicuramente un indice alla posizione del singolo buffer VUOTO,
	// settiamolo come USO, in modo che qualsiasi altro processo non possa accedervi.
	pc->stato[i] = USO;
	// decrementiamo il numero di buffer vuoti, perché ne abbiamo settato uno come USO
	pc->num_liberi--;

	// a questo punto il processo corrente ha accesso al buffer i-esimo del pool di buffer in mutua esclusione
	// ció significa che il processo puó operare fuori dal monitor senza problemi di RACE CONDITION, lasciando 
	// cosí altri processi la possibilitá di entrane all'interno del monitor e operare a loro volta sulla shm,
	// in modo concorrente.
	leave_monitor(&(pc->m));

	// eseguiamo la produzione
	sleep(10); // per simulare una produzione pesante 
	pc->buffer[i] = valore_prodotto;
	printf("[PRODUTTORE] valore prodotto (%d)\n", valore_prodotto);

	// il processo quindi deve rientrare all'interno del monitor per segnare come OCCUPATO il buffer i-esimo
	// e lo deve fare in modo mutuamente esclusivo.

	enter_monitor(&(pc->m));
	pc->num_occupati++;
	pc->stato[i] = OCCUPATO;

	// prima di uscire dal monitor deve segnalare eventuali consumatori che sono in attesa sulla condition variable VARCOND_CONSUMATORI
	// che attendo che si occupi un buffer 
	signal_condition(&(pc->m), VARCOND_CONSUMATORI);

	leave_monitor(&(pc->m));
	



}	
