#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

    // inizializzo memorie condivise per vettore e buffer
    key_t key = IPC_PRIVATE;
    int ds_coda = shmget(key, sizeof(VettoreProdCons), IPC_CREAT | 0664);
    int ds_buffer = shmget(key, sizeof(BufferMutuaEsclusione), IPC_CREAT | 0664);

    VettoreProdCons* vettore = (VettoreProdCons*) shmat(ds_coda, NULL, 0);
    BufferMutuaEsclusione* buffer = (BufferMutuaEsclusione*) shmat(ds_buffer, NULL, 0);

    // inizializzo strutture

    inizializza_buffer(buffer);
    inizializza_vettore(vettore);

    // creo processi figli

    pid_t pid;

    for(int i=0; i<NUM_PRODUTTORI; i++) {
        pid = fork();
        if (pid == 0) {
            produttore(vettore);
            exit(0);
        }
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {
        pid = fork();
        if (pid == 0) {
            consumatore(vettore, buffer);
            exit(0);
        }
        
    }


    // attendo terminazione figli
    for (int i = 0; i < NUM_CONSUMATORI + NUM_PRODUTTORI; i++) {
        wait(NULL);
    }

    
    // rimuovo strutture

    rimuovi_buffer(buffer);
    rimuovi_vettore(vettore);

    printf("APPLICAZIONE TERMINATA...\n");

    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        // produzione effettiva
        produci(vettore, valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        int valore;

        // consumo e aggiornamento effettivi
        valore = consuma(vettore);

        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        aggiorna(buffer, valore);

    }
}
