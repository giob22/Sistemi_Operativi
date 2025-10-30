#include "semafori.h"
#include "procedure.h"

//gestione semafori
//gestione shm
//gestione lifecycle dei processi


#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>

void produttore(int* p, int ds_sem){
	
	wait_sem(ds_sem,SPAZIO_DISP);
	// SEZIONE CRITICA
	*p = rand() % 1000;
	printf("[PRODUTTORE] prodotto %d\n", *p);
	signal_sem(ds_sem,MESS_DISP);
}

void consumatore(int* p,int ds_sem){
	

	wait_sem(ds_sem, MESS_DISP);
	//SEZIONE CRITICA
	printf("[CONSUMATORE] consumato %d\n", *p);
	signal_sem(ds_sem, SPAZIO_DISP);



}


