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
#include <sys/types.h>

#include <stdio.h>


//eventuali fork/clone di processi figli avranno una copia esatta dell'area .text di questa applicazione

int main(){

    srand(time(NULL));

	// istanziare shm
	// istanziare sem per disciplinare prod cons
	
	key_t shm_key = IPC_PRIVATE;

    // istanzia una shm tale per cui posso contenere una struct poll_buffer
	// int ds_shm = shmget(shm_key, sizeof(int), IPC_CREAT | 0644);
    printf("sizeof(struct pool_buffer): %ld\n", sizeof(struct pool_buffer));
	int ds_shm = shmget(shm_key, sizeof(int), IPC_CREAT | 0644);


    struct pool_buffer * shm_ptr = (struct pool_buffer *) shmat(ds_shm, NULL, 0); // condiviso tra tutti i processi che erediteranno dal processo
    // prima abbiamo usato un singolo buffer
    //*shm_ptr = 0;
    shm_ptr->testa = 0;
    shm_ptr->coda = 0;
    

    for (int i = 0; i < DIM_BUFFER; i++)
    {
        shm_ptr->buffer[i] = 0;
    }
    

    // crea l'array semaforico per gestire prod e cons

    key_t sem_key = IPC_PRIVATE; //anche il semaforo è un area di memoria condivisa 
    int ds_sem = semget(sem_key, 4, IPC_CREAT | 0644);

    // inizializzare i semafori, altrimenti non siamo sicuri del valore di sem_num
    semctl(ds_sem, MUTEX_PROD, SETVAL, 1);
    semctl(ds_sem, MUTEX_CONS, SETVAL, 1);
    // dobbiamo gestire la competizione tra prod e cons multipli
    
    // dobbiamo gestire la cooperazione tra prod e cons
    semctl(ds_sem, SPAZIO_DISP, SETVAL, DIM_BUFFER);
    semctl(ds_sem, MESS_DISP, SETVAL, 0);

    for(int i = 0; i< NUM_CONSUMATORI; i++){
    	pid_t pid = fork();
        if (pid == 0){
            printf("[PID: %d] CONSUMATORE avviato \n", getpid());
            consumatore(shm_ptr, ds_sem);
            exit(1);// evito la forkbomb
        }
    }
    for(int i = 0; i< NUM_PRODUTTORI; i++){
    	pid_t pid = fork();
        if (pid == 0){
            printf("[PID: %d] PRODUTTORE avviato \n", getpid());
            produttore(shm_ptr, ds_sem);
            exit(1);
        }
    }

    //pid_t pid = fork();
    // creo consumatore
    
    
    
    //pid = fork();
    // creo produttore
    /*if (pid == 0){
        printf("[PID: %d] PRODUTTORE avviato \n", getpid());
        consumatore(shm_ptr, ds_sem);
        exit(1);
    }
    
    wait(NULL);
    wait(NULL);
    */

    for (int i = 0; i < NUM_CONSUMATORI + NUM_PRODUTTORI; i++)
    {
        wait(NULL);
    }
    printf("processo terminato...\n");
    

    // marcare shm e sem come da rimuovere 
    semctl(ds_sem, 0, IPC_RMID);
    shmctl(ds_shm, IPC_RMID, NULL);


	return 0;
}
