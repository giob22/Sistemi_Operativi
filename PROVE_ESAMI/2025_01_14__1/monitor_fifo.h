
#ifndef _HEADER_
#define _HEADER_

#include "monitor_signal_continue.h"

#define DIM 3



#define PROD 0
#define CONS 1

#define VUOTO 0
#define PIENO 1
#define USO 3

typedef struct {
    
int buffer[DIM];

int ultimo_produttore_arrivato;
int ultimo_produttore_avviato;

// TBD: Aggiungere ulteriori variabili per la sincronizzazione 

short stato[DIM];

int num_occupati;
int num_liberi;

Monitor m;

} MonitorProdConsFIFO;

void inizializza(MonitorProdConsFIFO * p);
void produci(MonitorProdConsFIFO * p, int valore);
int consuma(MonitorProdConsFIFO * p);
void distruggi(MonitorProdConsFIFO * p);

#endif



