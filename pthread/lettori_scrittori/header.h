#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_LETTORI 2
#define NUM_SCRITTORI 2
#define RIPETI 5


typedef struct{
	
	// buffer per la lettura e scrittura
	int buffer;


	// variabili di stato per gestire la sincronizzazione 
	int num_lettori;
	int num_scrittori;
	int num_scrittori_s; // numero di scrittori sospesi sulla loro condition variables

	pthread_mutex_t mutex;
	pthread_cond_t ok_scritt;
	pthread_cond_t ok_lett;	
}Buffer;


void* scrittore(void* p);
void* lettore(void* p);

void scrivi(Buffer* p);
int leggi(Buffer* p);


