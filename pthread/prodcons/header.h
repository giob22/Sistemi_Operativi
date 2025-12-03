#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef DEBUG

#define LOG(string, ...) printf("[LOG] "  string  "\n", ##__VA_ARGS__)

#endif

#define PIENO 1
#define VUOTO 0

#define NUM_VALORI 4

struct prodcons{

    // risorse condivisa
    int buffer;

    // le var. a supporto della gestione delle cooperazione

    short stato;

    // le var. di sincronizzazione della pthread.h, monitor pthread
    // mutex, 2 var cond

    pthread_mutex_t mutex; // mutex per l'accesso al monitor
    pthread_cond_t ok_produci_cond; // var cond per produttori
    pthread_cond_t ok_consuma_cond; // var cond per consumatori
};


void* consumatore(void*);
void* produttore(void*);


void produci(struct prodcons*);
int consuma(struct prodcons*);
