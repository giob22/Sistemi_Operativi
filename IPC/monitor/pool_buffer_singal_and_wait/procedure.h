#include "monitor_hoare.h"

// definiamo due identificatori, uno per ogni variabile condition

#define VARCOND_CONSUMATORI 0
#define VARCOND_PRODUTTORI 1


#define DIM_BUFFER 15



#define VUOTO 0
#define OCCUPATO 1
#define USO 2

typedef struct {
	int buffer[DIM_BUFFER];

	int stato[DIM_BUFFER];

	// variabili di stato che mi descrivono la quantità corrente di spazi liberi e spazi occupati
	// all'interno del poll di buffer (buffer)
	int num_occupati;
	int num_liberi;

	Monitor m;
} Buffer;



int consuma(Buffer*);
void produci(Buffer*, int);


