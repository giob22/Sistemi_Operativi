#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "monitor_fifo.h"

#define PROCESSI_PRODUTTORI 5
#define PROCESSI_CONSUMATORI 2

#define NUMERO_PRODUZIONI 4
#define NUMERO_CONSUMAZIONI 10

void consumatore(MonitorProdConsFIFO * p);
void produttore(MonitorProdConsFIFO * p);

int main() {

    /* TBD: Creare l'oggetto monitor, utilizzando la funzione "inizializza()" */

    // creo la shm che deve essere condivisa dai processi
    key_t key_shm = IPC_PRIVATE;
    int shmid = shmget(key_shm, sizeof(MonitorProdConsFIFO), IPC_CREAT | 0664);

    if (shmid < 0){
        perror("Errore nella creazione della shm\n");
        exit(-1);
    }

    // attach della shm
    MonitorProdConsFIFO* p = (MonitorProdConsFIFO*) shmat(shmid, NULL, 0);

    // una volta ottenuto il puntatore alla memoria condivisa, inizializzo il monitor
    inizializza(p);


    /* TBD: Avviare i processi produttori, facendogli eseguire la funzione "produttore()"
            e i processi consumatori, facendogli eseguire la funzione "consumatore()"
     */
    
    // inizializzo i vari processi produttori e consumatori
    pid_t pid;
    // CONSUMATORI
    for (int i = 0; i < PROCESSI_CONSUMATORI; i++){
        pid = fork();
        if (pid == 0){
            // contesto consumatore
            consumatore(p);
            exit(0);
        }
    }

    // PRODUTTORI
    for (int i = 0; i < PROCESSI_PRODUTTORI; i++){
        pid = fork();
        if (pid == 0){
            // contesto del processo produttore
            produttore(p);
            exit(0);
        }
    }

    // il processo padre attende la terminazione di tutti i processi figli per concludere l'esecuzione ed eliminare 
    // la shm e il monitor

    for (int i = 0; i < PROCESSI_CONSUMATORI + PROCESSI_PRODUTTORI; i++){
        wait(NULL);
    }
    /* TBD: Rimuovere l'oggetto monitor, utilizzando la funzione "distruggi()" */

    distruggi(p);
    // dopo aver distrutto il monitor marco come eliminabile la shm

    shmctl(shmid, IPC_RMID, 0);
}


void produttore(MonitorProdConsFIFO * p) {

    srand(getpid());

    for(int i=0; i<NUMERO_PRODUZIONI; i++) {

        int valore = rand() % 100;

        printf("[PRODUTTORE %d] Produzione valore %d\n", getpid(), valore);

        /* TBD: Chiamare la funzione "produci()" del monitor */

        produci(p,valore);

    }

}

void consumatore(MonitorProdConsFIFO * p) {

    for(int i=0; i<NUMERO_CONSUMAZIONI; i++) {

        printf("[CONSUMATORE %d] Inizio consumazione\n", getpid());

        /* TBD: Chiamare la funzione "consuma()" del monitor */
        int valore = consuma(p);


        printf("[CONSUMATORE %d] Consumazione valore %d\n", getpid(), valore);
    }

}