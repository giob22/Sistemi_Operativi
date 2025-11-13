#include "monitor_hoare.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>



// definiamo due identificatori, uno per ogni variabile condition

#define VARCOND_LETTORI 0
#define VARCOND_SCRITTORI 1



typedef struct {
	int buffer;

	// variabili di stato che sono necessarie per costruire le condizioni di sincronizzazione
	int num_lettori;
	int num_scrittori;

	Monitor m;
} Buffer;

 



int lettura(Buffer*);
void scrittura(Buffer*, int);


