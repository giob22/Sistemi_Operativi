#include "procedure.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>


int stampa_risultato(MonitorRisultati * ls) {

	int risultato;

	// TODO: consumo il valore

	enter_monitor(&(ls->m));

	// condizione di sincronizzazione
	if (ls->dim == 0){
		// il lettore si deve sospendere
		wait_condition(&(ls->m), VARCOND_CONSUMATORI);
	}

	// riattivato

	risultato = ls->buffer[ls->testa];
	ls->testa = (ls->testa + 1)%DIM;
	ls->dim--;

	// risveglia eventuali produttori
	signal_condition(&(ls->m), VARCOND_PRODUTTORI);

	leave_monitor(&(ls->m)); 

	return risultato;
			
		
}


void inserisci_risultato(MonitorRisultati * ls, int valore) {

	// TODO: produco il valore
	enter_monitor(&(ls->m));

	// condizione di sincronizzazione
	if(ls->dim == DIM){
		// il produttore deve sospendersi
		wait_condition(&(ls->m), VARCOND_PRODUTTORI);
	}

	ls->buffer[ls->coda] = valore;
	ls->coda = (ls->coda + 1)%DIM;
	ls->dim++;

	// risveglia eventuali consumatori
	signal_condition(&(ls->m), VARCOND_CONSUMATORI);

	leave_monitor(&(ls->m));


}
void stampa(MonitorRisultati * ls) {
	int val;
	for (int i = 0 ; i < 4; i++) {
		// TODO: stampo il valore appena consumato
		val = stampa_risultato(ls);
		printf("[STAMPA] %d\n", val);
	}
}

void filtro(MonitorRisultati * ls, int coda){
	Messaggio m;
	int valore = 0;
	for (int i = 0 ; i < 4; i++){
		// TODO: filtro i valori prelevati dalla mailbox: se sono pari, li inserisco, altrimenti inserisco 0
		if(msgrcv(coda, &m, sizeof(Messaggio) - sizeof(long), MESSAGGIO, 0) < 0){
			perror("[Errore] ricezione del messaggio\n");
			exit(-1);
		}
		printf("[FILTRO] messaggio ricevuto tipo: %ld, valore: %d\n", m.type, m.val);
		valore = m.val;
		(valore%2 == 0) ? inserisci_risultato(ls, valore) : inserisci_risultato(ls, 0);
	}
}
void generazione(int coda) {
	srand(getpid()*time(NULL));
	Messaggio m;
	sleep(2);
	for (int i = 0 ; i < 4; i++) {
		
		// TODO: genero dei numeri casuali tra 1 e 20 e li invio alla mailbox
		int valore = rand()%20 + 1;
		m.type = MESSAGGIO;
		m.val = valore;
		printf("[GEN] valore generato: %d\n", valore);
		msgsnd(coda, &m, sizeof(Messaggio) - sizeof(long), 0);

	}
}
