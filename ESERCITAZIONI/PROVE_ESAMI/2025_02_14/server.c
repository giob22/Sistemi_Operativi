#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include "header_prodcons.h"
#include "header_msg.h"
#include "semafori.h"


void ricevi_da_collettore(int id_queue_server, BufferProdCons * b);
void visualizza(BufferProdCons * b);
void produci_valore(BufferProdCons * b, int valore);
int consuma_valore(BufferProdCons * b);

int main () {

    int sem_key = IPC_PRIVATE;
    int sem_id = semget(sem_key, 4, IPC_CREAT | 0664);
    //TODO inizializzazione semafori

    semctl(sem_id, SPAZIO, SETVAL, DIM_BUFF);
    semctl(sem_id, MESS, SETVAL, 0);
    semctl(sem_id, MUTEXC, SETVAL, 1);
    semctl(sem_id, MUTEXP, SETVAL, 1);

    //TODO inizializzazione struttura condivisa
    int shmid = shmget(IPC_PRIVATE, sizeof(BufferProdCons), IPC_CREAT | 0664);

    BufferProdCons * b = shmat(shmid, NULL, 0);

    b->semid = sem_id;
    b->coda = 0;
    b->testa = 0;

    key_t chiave_server = ftok("./main.c",'b');
    int id_queue_server = msgget(chiave_server, IPC_CREAT | 0664);


    //TODO: Creazione processo "ricevi_da_collettore"
    pid_t pid;
    pid = fork();
    if(pid == 0){
        ricevi_da_collettore(id_queue_server, b);
        exit(0);
    }
    
    //TODO: Creazione 2 processi "visualizza"

    for(int i = 0; i < 2; i++){
        pid = fork();
        if(pid == 0){
            visualizza(b);
            exit(0);
        }
    }

    //TODO: attesa processi figli

    for(int i = 0; i < 3; i++){
        wait(NULL);
    }
    //TODO: rimozione risorse IPC

    semctl(sem_id, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(id_queue_server, IPC_RMID, NULL);

    return 0;
}

void ricevi_da_collettore(int id_queue_server, BufferProdCons * b){

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        //TODO: ricezione messaggio con media da collettore
        messaggio_collettore c;
        msgrcv(id_queue_server, &c, sizeof(messaggio_collettore) - sizeof(long), TYPE_COLLETTORE, 0);

        int media = c.val;

        produci_valore(b, media);

    }

}

void visualizza(BufferProdCons * b) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE/2; i++) {
        sleep(2);
        int media = consuma_valore(b);

        printf("[VISUALIZZATORE (server)] Visualizzo valore medio corrente=%d\n", media);
    
    }

}


void produci_valore(BufferProdCons * b, int valore) {


    Wait_Sem(b->semid, SPAZIO);

    // è presente dello spazio nel buffer
    Wait_Sem(b->semid, MUTEXP);

    b->valore[b->testa] = valore;
    b->testa = (b->testa + 1)%DIM_BUFF;
    Signal_Sem(b->semid, MUTEXP);

    Signal_Sem(b->semid, MESS);
    //TODO: implementare produttore

}

int consuma_valore(BufferProdCons * b) {

    Wait_Sem(b->semid, MESS);

    Wait_Sem(b->semid, MUTEXC);

    int valore = b->valore[b->coda];
    b->coda = (b->coda+1)%DIM_BUFF;

    Signal_Sem(b->semid, MUTEXC);

    Signal_Sem(b->semid, SPAZIO);

    return valore;

    //TODO: implementare consumatore

}
