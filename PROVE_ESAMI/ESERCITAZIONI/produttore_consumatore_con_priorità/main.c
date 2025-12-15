#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

int main(){
    // creo la shm
    key_t key_shm = IPC_PRIVATE;

    int shm_id = shmget(key_shm, sizeof(PriorityProdCons), IPC_CREAT | 0664);

    if (shm_id < 0){
        perror("Errore nella creazione della shm");
        exit(-2);
    }

    // faccio l'attach
    PriorityProdCons* p = (PriorityProdCons*)shmat(shm_id, NULL, 0);

    // una volta creata la shm, la inizializzo

    inizializza_prod_cons(p);

    // una volta inizializzata creo i vari processi
    // 1 processo produttore ad alta priorità
    // 3 processi produttore a bassa priorità
    // 1 processo consumatore

    pid_t pid;

    for (int i = 0; i < 4; i++){
        pid = fork();
        if(pid == 0){
            // contesto del processo figlio
            srand(time(NULL) * getpid());
            if(i == 0){
                // produttore ad alta priorità
                printf("produttore ad alta priorità attivato\n");
                for(int j = 0; j < RIPETI_PROD; j++){
                    sleep(2);
                    produci_alta_prio(p);
                }
            }else{
                // produttore a bassa priorità
                printf("produttore ad bassa priorità attivato\n");
                for(int j = 0; j < RIPETI_PROD; j++){
                    sleep(1);
                    produci_bassa_prio(p);
                }
            }
            exit(0);
        }
    }

    // creo il processo consumatore
    sleep(1);
    pid = fork();
    if (pid == 0){
        printf("consumatore avviato\n");
        for(int i = 0; i < RIPETI_CONS; i++){
            sleep(1);
            consuma(p);
        }
        exit(0);
    }

    for(int i = 0; i < 1+3+1; i++){
        wait(NULL);
    }

    printf("PROCESSO TERMINATO...\n");


    // rimuovo il monitor e marco come eliminabile la shm
    rimuovi_prod_cons(p);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}