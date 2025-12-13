#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "header.h"

// un thread non può operare se è presente già un altro thread nel mutex

typedef struct {

    int * vettore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */

    pthread_mutex_t mutex;

} risorsa_database;

///////////////////////////////// da vedere sulla soluzione
typedef struct {

    /* TBD: Utilizzare questa struttura dati per il
            passaggio dei parametri dal thread principale ai
            thread worker.
     */
    risorsa_database* r;

    int msqid_req_2;
    int msqid_res_2;
    

} parametri_worker;


void * worker(void * x);


#define THREAD_WORKER 3


int main() {

    // risorsa gestita dal database (vettore di interi)

    risorsa_database * r = malloc(sizeof(risorsa_database));

    r->vettore = malloc(sizeof(int) * DIM_VETTORE);

    for(int i=0; i<DIM_VETTORE; i++) {
        r->vettore[i] = 0;
    }


    /* TBD: Completare l'inizializzazione delle variabili di sincronizzazione */
    pthread_mutex_init(&(r->mutex), NULL);


    /* TBD: Ottenere gli identificativi delle code di messaggi */

    
    key_t k_queue_req_2 = ftok("./start.c", 'c');
    key_t k_queue_res_2 = ftok("./start.c", 'd');

    int msqid_req_2 = msgget(k_queue_req_2, IPC_CREAT | 0664);
    int msqid_res_2 = msgget(k_queue_res_2, IPC_CREAT | 0664);



    /* TBD: Creare 3 thread worker, e utilizzare la struttura "parametri_worker"
            per passare ai thread il puntatore alla risorsa e gli
            identificativi delle code di messaggi.
     */

    pthread_t threads[THREAD_WORKER];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(int i=0; i<THREAD_WORKER; i++) {

        /* TBD */
        parametri_worker* p = (parametri_worker*) malloc(sizeof(parametri_worker));
        p->r = r;
        p->msqid_req_2 = msqid_req_2;
        p->msqid_res_2 = msqid_res_2;

        pthread_create(&threads[i], &attr, worker, (void*)p);
    }

    /* TBD: Attendere la terminazione dei thread worker */

    for (int i = 0; i < THREAD_WORKER; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&(r->mutex));



    free(r->vettore);
    free(r);

}



void * worker(void * x) {

    /* TBD: Completare il passaggio dei parametri */

    parametri_worker* p = (parametri_worker*)x;

    risorsa_database * r = /* TBD */p->r;

    
    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta */
        messaggio_richiesta req_from_server;
        msgrcv(p->msqid_req_2, &req_from_server, sizeof(messaggio_richiesta)-sizeof(long), 0, 0);

        int tipo_richiesta = req_from_server.op/* TBD */;
        int valore = /* TBD */req_from_server.valore;
        int posizione = /* TBD */req_from_server.pos;
        int pid_server = /* TBD */req_from_server.type;

        printf("[DB] Ricevuta richiesta (tipo=%s, pid=%d, posizione=%d, valore=%d)\n", (tipo_richiesta == LETTURA) ? "LETTURA" : "SCRITTURA", pid_server, posizione, valore);


        int valore_risposta;

        /* TBD: Aggiungere la sincronizzazione per l'accesso alla risorsa condivisa */
        pthread_mutex_lock(&(r->mutex));
        if(tipo_richiesta == LETTURA/* TBD: Verificare se è una richiesta di LETTURA */) {

            valore_risposta = r->vettore[posizione];

        }
        else if(tipo_richiesta == SCRITTURA/* TBD: Verificare se è una richiesta di SCRITTURA */) {

            r->vettore[posizione] = valore;
            valore_risposta = 0;

        }
        else {
            printf("[DB] Tipo richiesta non valido\n");
        }



        /* TBD: Inviare un messaggio di risposta, includendo la variabile "valore_risposta" */
        messaggio_risposta res_for_server;
        res_for_server.valore = valore_risposta;
        res_for_server.type = pid_server;
        msgsnd(p->msqid_res_2, &res_for_server, sizeof(messaggio_risposta) - sizeof(long), 0);
        pthread_mutex_unlock(&(r->mutex));
    }

    free(p);

    return NULL;

}