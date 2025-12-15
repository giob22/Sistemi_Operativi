#include "monitor_signal_continue.h"
#include <stdio.h>
#include <stdlib.h>

#define PROD_HP 0
#define PROD_LP 2
#define CONS 1


/*
abbiamo bisogno di 3 condition variables perché ci sono 3 condizioni di sincronizzazione diverse
- un consumatore non può consumare se il buffer è vuoto
- un produttore non può produrre se il buffer è pieno
- un produttore a bassa priorità non può produrre se c'è un produttore ad alta priorità in attesa
*/ 
#define DIM_BUFFER 3

#define RIPETI_PROD 3
#define RIPETI_CONS 12



typedef struct {
    int buffer[DIM_BUFFER];
    int testa; int coda;
    int dim; // indica se il buffer circolare è pieno o meno
    Monitor m;      // utilizzare la libreria di procedure allegate
} PriorityProdCons; 


void inizializza_prod_cons(PriorityProdCons * p);
void produci_alta_prio(PriorityProdCons * p);
void produci_bassa_prio(PriorityProdCons * p);
void consuma(PriorityProdCons * p);
void rimuovi_prod_cons(PriorityProdCons * p);