// implementazione di un pool_buffer con vettore di stato

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef DEBUG

#define LOG(string, ...) printf("[LOG] "  string  "\n", ##__VA_ARGS__)

#endif

#define MAX_DIM 10

#define PIENO 1
#define VUOTO 0
#define USO 2

#define NUM_RIPETIZIONI 10

struct prodcons{

    // risorse condivisa
    int buffer[MAX_DIM];

    // le var. a supporto della gestione delle cooperazione

    short stato[MAX_DIM];
	short num_occupati;
	short num_liberi;


    // le var. di sincronizzazione della pthread.h, monitor pthread
    // mutex, 2 var cond

    pthread_mutex_t mutex; // mutex per l'accesso al monitor
    pthread_cond_t ok_produci_cond; // var cond per produttori
    pthread_cond_t ok_consuma_cond; // var cond per consumatori
};

// ho sempre due condizione che in questa implementazione cambiano
// infatti saranno:
// 1) un produttore non piò produrre se il buffer è pieno
// 2) un lettore non può consumare se il buffer è vuoto


void* consumatore(void*);
void* produttore(void*);


void produci(struct prodcons*);
int consuma(struct prodcons*);
