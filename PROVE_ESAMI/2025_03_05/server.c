#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "header_msg.h"
#include "header_prodcons.h"

struct parametri_thread_1 {

    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b1, e gli identificativi delle code di messaggi
    */
    struct buffer* b1;
    int msqid_rts;
    int msqid_ots;
    int msqid_req;
    
};

struct parametri_thread_2 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
    includendo il buffer b1 e il buffer b2
    */
    struct buffer* b1;
    struct buffer* b2;
};

struct parametri_thread_3 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
    includendo il buffer b2
    */

    struct buffer* b2;
};

void * thread_1(void *);
void * thread_2(void *);
void * thread_3(void *);

int main() {

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_rts = ftok(".", 'a');
    key_t key_ots = ftok(".", 'b');
    key_t key_req = ftok(".", 'c');

    int msqid_rts = msgget(key_rts, IPC_CREAT | IPC_EXCL | 0664);
    if (msqid_rts < 0)
    {
        // significa che la coda già esiste e non ha potuto crearla in esclusiva
        if (errno == EEXIST)
        {
            // faccio l'attach alla coda esistente
            msqid_rts = msgget(key_rts, 0);
        }else{
            perror("ERRORE");
            exit(2);
        }
    }else{
        // se crea la coda significa che non è stata creata in precedenza dal processo start, quindi faccio terminare il processo
        msgctl(msqid_rts, IPC_RMID, NULL);
        perror("ERRORE");
        exit(2);
    }
    
    int msqid_ots = msgget(key_ots, IPC_CREAT | IPC_EXCL | 0664);
    if (msqid_ots < 0)
    {
        // significa che la coda già esiste e non ha potuto crearla in esclusiva
        if (errno == EEXIST)
        {
            // faccio l'attach alla coda esistente
            msqid_ots = msgget(key_ots, 0);
        }else{
            perror("ERRORE");
            exit(2);
        }
    }else{
        // se crea la coda significa che non è stata creata in precedenza dal processo start, quindi faccio terminare il processo
        msgctl(msqid_ots, IPC_RMID, NULL);
        perror("ERRORE");
        exit(2);
    }
    int msqid_req = msgget(key_req, IPC_CREAT | IPC_EXCL | 0664);
    if (msqid_req < 0)
    {
        // significa che la coda già esiste e non ha potuto crearla in esclusiva
        if (errno == EEXIST)
        {
            // faccio l'attach alla coda esistente
            msqid_req = msgget(key_req, 0);
        }else{
            perror("ERRORE");
            exit(2);
        }
    }else{
        // se crea la coda significa che non è stata creata in precedenza dal processo start, quindi faccio terminare il processo
        msgctl(msqid_req, IPC_RMID, NULL);
        perror("ERRORE");
        exit(2);
    }
    
    // TODO da aggiungere la verifica

    /* TBD: Inizializzare il puntatore alla struttura dati */
    struct buffer * b1 = (struct buffer*) malloc(sizeof(struct buffer)); 
    /* TBD: Inizializzare il puntatore alla struttura dati */
    if (b1 == NULL)
    {
        perror("Errore");
        exit(1);
    }
    
    struct buffer * b2 = (struct buffer*) malloc(sizeof(struct buffer)); 
    if (b2 == NULL)
    {
        perror("Errore");
        exit(1);
    }
    
    buffer_init(b1);
    buffer_init(b2);
    
    
    struct parametri_thread_1 * p1 = (struct parametri_thread_1*) malloc(sizeof(struct parametri_thread_1));
    if (p1 == NULL)
    {
        perror("Errore");
        exit(1);
    }
    /* TBD: Inizializzare il puntatore alla struttura dati */
    
    /* TBD: Inizializzare i valori nella struttura "p1" */
    p1->b1 = b1;
    p1->msqid_ots = msqid_ots;
    p1->msqid_req = msqid_req;
    p1->msqid_rts = msqid_rts;


    struct parametri_thread_2 * p2 = (struct parametri_thread_2*) malloc(sizeof(struct parametri_thread_2));/* TBD: Inizializzare il puntatore alla struttura dati */
    if (p2 == NULL)
    {
        perror("Errore");
        exit(1);
    }

    /* TBD: Inizializzare i valori nella struttura "p2" */
    p2->b1 = b1;
    p2->b2 = b2;


    struct parametri_thread_3 * p3 = (struct parametri_thread_3*) malloc(sizeof(struct parametri_thread_3));/* TBD: Inizializzare il puntatore alla struttura dati */
    if (p3 == NULL)
    {
        perror("Errore");
        exit(1);
    }
    /* TBD: Inizializzare i valori nella struttura "p3" */
    p3->b2 = b2;


    /* TBD: Creare i 3 thread, passandogli le 3 strutture dati con i parametri */
    pthread_t threads[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, thread_1, (void*)p1);
    pthread_create(&threads[1], &attr, thread_2, (void*)p2);
    pthread_create(&threads[2], &attr, thread_3, (void*)p3);

    /* TBD: Attendere la terminazione dei 3 thread */

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    buffer_destroy(b1);
    buffer_destroy(b2);
    free(b1);
    free(b2);
    free(p1);
    free(p2);
    free(p3);

}


void * thread_1(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_1* p = (struct parametri_thread_1*)x;
    
    struct rts rts;
    struct ots ots;
    struct richiesta r;
    
    
    for(int i=0; i<3; i++) {
        
        /* TBD: Ricevere il messaggio "request to send" */
        int ret = msgrcv(p->msqid_rts, &rts, sizeof(struct rts) - sizeof(long), REQUEST_TO_SEND,0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }
        
        
        printf("[THREAD 1] Ricevuto messaggio RTS\n");
        
        
        
        /* TBD: Inviare il messaggio "ok to send" */
        ots.type = OK_TO_SEND;
        ret = msgsnd(p->msqid_ots, &ots, sizeof(struct ots) - sizeof(long), 0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }        
        printf("[THREAD 1] Inviato messaggio OTS\n");
        
        
        
        
        /* TBD: Ricevere il messaggio con il valore */
        ret = msgrcv(p->msqid_req, &r, sizeof(struct richiesta) - sizeof(long), MESSAGE, 0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }
        
        int valore = r.value;
        
        printf("[THREAD 1] Ricevuto messaggio MSG con valore %d\n", valore);
        
        
        buffer_produci(p->b1, valore);
        
        printf("[THREAD 1] Inserito valore %d nel buffer 1\n", valore);
    }
    
    pthread_exit(NULL);
    
}

void * thread_2(void * x) {
    
    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_2* p = (struct parametri_thread_2*)x;
    
    int valore;
    
    for(int i=0; i<3; i++) {
        
        valore = buffer_consuma(p->b1);
        
        
        printf("[THREAD 2] Prelevato valore %d dal buffer 1\n", valore);
        
        
        sleep(2);
        
        
        valore = valore * 2;
        
        buffer_produci(p->b2, valore);
        
        printf("[THREAD 2] Inserito valore %d nel buffer 2\n", valore);
    }
    
    pthread_exit(NULL);
}

void * thread_3(void * x) {
    
    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_3* p = (struct parametri_thread_3*)x;
    
    int valore;

    for(int i=0; i<3; i++) {

        valore = buffer_consuma(p->b2);

        printf("[THREAD 3] Prelevato valore %d dal buffer 2\n", valore);

        sleep(1);

        valore = valore + 1;

        printf("[THREAD 3] Valore finale: %d\n", valore);

    }

    pthread_exit(NULL);
}

