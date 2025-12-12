#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "header.h"

#define NUM_CONSUMI 10

int main() {

    /*
        TODO: inizializzazione segmenti di memoria e strutture condivisi
    */

    key_t key = IPC_PRIVATE;

    int shmid = shmget(key, sizeof(MonitorProdCons), IPC_CREAT | 0664);

    // attach del segmento

    MonitorProdCons* pc = (MonitorProdCons *) shmat(shmid, NULL, 0);

    // inizializzo il vettore di buffer

    inizializza_vettore(pc);


    // ottengo la shm per il buffer

    int shmid_b = shmget(key, sizeof(BufferBabbo), IPC_CREAT | 0664);

    // attach
    BufferBabbo* bb = (BufferBabbo*) shmat(shmid_b, NULL, 0);

    // inizializzo il buffer
    inizializza_buffer(bb);

    // creo i processi figlio che opereranno

    pid_t pid_p, pid_c1, pid_c2, pid_b;

    /*
        TODO: creazione di 4 figli: un Produttore, un Consumatore1, un Consumatore2 e BabboNatale
    */
    pid_p = fork();
    if (pid_p == 0){
        // contesto processo fabbricatore
        Produttore(pc);
        exit(0);
    }
    sleep(3);
    pid_c1 = fork();
    if (pid_c1 == 0){
        // contesto processo fabbricatore
        for(int i = 0; i < 10; i++)
            Consumatore1(pc,bb);
        exit(0);
    }
    pid_c2 = fork();
    if (pid_c2 == 0){
        // contesto processo fabbricatore
        for(int i = 0; i < 10; i++)
            Consumatore2(pc,bb);
        exit(0);
    }
    pid_b = fork();
    if (pid_b == 0){
        // contesto processo fabbricatore
        BabboNatale(bb);
        exit(0);
    }

    



    /*
        TODO: attesa terminazione figli
    */

    waitpid(pid_p,NULL,0);
    waitpid(pid_b,NULL,0);
    waitpid(pid_c2,NULL,0);
    waitpid(pid_c1,NULL,0);

    /*
        TODO: distruzione strutture
    */

    rimuovi_vettore(pc);

    shmctl(shmid, IPC_RMID, NULL);

    rimuovi_buffer(bb);

    shmctl(shmid_b, IPC_RMID, NULL);

    printf("FINE APPLICAZIONE...\n");


    return 0;
}