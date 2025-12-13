#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include "header.h"

#define NUM_WORKER 3

static struct Coda * coda;
static int ds_sem;
static pthread_t threads[NUM_WORKER];
static int end = 0;

// struttura per passaggio parametri ai thread
typedef struct {
	pthread_t id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	//TODO aggiungere codice per inizializzare Count
	pthread_mutex_init(&(c->mutex), NULL);
}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
	pthread_mutex_destroy(&(c->mutex));
	free(c);
}


void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = (parametri*) arg;
	//TODO: completare il passaggio dei parametri
	
	while (!end) {
		//TODO completare il codice del worker
		// worker preleva dalla coda con funzione "consuma"
		int valore = consuma(coda, ds_sem);
		
		// aggiornamento (eventuale) variabile end se valore ricevuto = -1
		if (valore == -1){
			end = 1;
			printf("Valore di prenotazioni raggiunto: %d\n", par->count->num_prenotazioni);
		}
		else{
			// aggiornamento Count in mutua esclusione
			pthread_mutex_lock(&(par->count->mutex));
			
			par->count->num_prenotazioni += valore;
			
			pthread_mutex_unlock(&(par->count->mutex));
		}
	}
    //TODO uscita dal thread
	pthread_exit(NULL);
}



int main(){

	int i,k;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok("./main.c", 'a');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT | 0664);

	coda = shmat(ds_shm, NULL, 0);
	
	//ottenimento semafori

	key_t key_sem;
	key_sem = ftok("./main.c", 'b');

	ds_sem = semget(key_sem, 4, IPC_CREAT | 0664);

	//TODO Creazione e Inizializzazione struttura locale
	Count* count = (Count*) malloc(sizeof(Count));
	init_buffer(count);
	
	parametri param[NUM_WORKER];
	

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		//TODO creazione threads
		param[i].count = count;
		
		pthread_create(&(param[i].id), &attr, worker, (void*)&param[i]);
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  //TODO join 
	  pthread_join(param[k].id, NULL);
	}
	remove_buffer(count);
	//TODO rimozione struttura locale
	printf("[SERVER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

