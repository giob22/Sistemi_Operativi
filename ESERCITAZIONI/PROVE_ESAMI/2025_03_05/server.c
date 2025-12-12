#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "header_msg.h"
#include "header_prodcons.h"

struct parametri_thread_1
{

    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b1, e gli identificativi delle code di messaggi
    */

    int request;
    int ok_to_send;
    int msqid;

    struct buffer *b1;
};

struct parametri_thread_2
{
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
    includendo il buffer b1 e il buffer b2
    */

    struct buffer *b1;
    struct buffer *b2;
};

struct parametri_thread_3
{
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
    includendo il buffer b2
    */

    struct buffer *b2;
};

void *thread_1(void *);
void *thread_2(void *);
void *thread_3(void *);

int main()
{

    /* Disabilita il buffering su stdout e stderr per scritture immediate */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    /* TBD: Ottenere gli identificativi delle code di messaggi */

    key_t kq1 = ftok(".start.c", 'a');
    key_t kq2 = ftok(".start.c", 'b');
    key_t kq3 = ftok("./start.c", 'c');

    int request = msgget(kq1, IPC_CREAT | 0664);
    int ok_to_send = msgget(kq2, IPC_CREAT | 0664);
    int msqid = msgget(kq3, IPC_CREAT | 0664);

    /* TBD: Inizializzare il puntatore alla struttura dati */
    struct buffer *b1 = (struct buffer *)malloc(sizeof(struct buffer));
    /* TBD: Inizializzare il puntatore alla struttura dati */
    struct buffer *b2 = (struct buffer *)malloc(sizeof(struct buffer));

    buffer_init(b1);
    buffer_init(b2);

    struct parametri_thread_1 *p1 = (struct parametri_thread_1 *)malloc(sizeof(struct parametri_thread_1));
    /* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p1" */
    p1->b1 = b1;
    p1->msqid = msqid;
    p1->ok_to_send = ok_to_send;
    p1->request = request;

    struct parametri_thread_2 *p2 = (struct parametri_thread_2 *)malloc(sizeof(struct parametri_thread_2));
    /* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p2" */
    p2->b1 = b1;
    p2->b2 = b2;

    struct parametri_thread_3 *p3 = (struct parametri_thread_3 *)malloc(sizeof(struct parametri_thread_3));
    /* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p3" */
    p3->b2 = b2;

    /* TBD: Creare i 3 thread, passandogli le 3 strutture dati con i parametri */

    pthread_t threads[3];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, thread_1, (void *)p1);
    pthread_create(&threads[1], &attr, thread_2, (void *)p2);
    pthread_create(&threads[2], &attr, thread_3, (void *)p3);

    /* TBD: Attendere la terminazione dei 3 thread */

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // eliminazione dei buffer e free dello heap
    buffer_destroy(b1);
    buffer_destroy(b2);
    free(b1);
    free(b2);
    free(p1);
    free(p2);
    free(p3);

    return 0;
}

void *thread_1(void *x)
{

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_1 *p = (struct parametri_thread_1 *)x;

    struct rts rts;
    struct ots ots;
    struct richiesta r;

    for (int i = 0; i < 3; i++)
    {

        /* TBD: Ricevere il messaggio "request to send" */

        msgrcv(p->request, &rts, sizeof(struct rts) - sizeof(long), RTS, 0);
        printf("[THREAD 1] Ricevuto messaggio RTS\n");

        ots.type = OTS;
        msgsnd(p->ok_to_send, &ots, sizeof(struct ots) - sizeof(long), 0);
        /* TBD: Inviare il messaggio "ok to send" */

        printf("[THREAD 1] Inviato messaggio OTS\n");

        msgrcv(p->msqid, &r, sizeof(struct richiesta) - sizeof(long), TYPE, 0);

        /* TBD: Ricevere il messaggio con il valore */

        int valore = r.valore;

        printf("[THREAD 1] Ricevuto messaggio MSG con valore %d\n", valore);

        buffer_produci(p->b1, valore);

        printf("[THREAD 1] Inserito valore %d nel buffer 1\n", valore);
    }

    pthread_exit(NULL);
}

void *thread_2(void *x)
{

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_2 *p = (struct parametri_thread_2 *)x;

    int valore;

    for (int i = 0; i < 3; i++)
    {

        valore = buffer_consuma(p->b1);

        printf("[THREAD 2] Prelevato valore %d dal buffer 1\n", valore);

        sleep(2);

        valore = valore * 2;

        buffer_produci(p->b2, valore);

        printf("[THREAD 2] Inserito valore %d nel buffer 2\n", valore);
    }

    pthread_exit(NULL);
}

void *thread_3(void *x)
{

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_3 *p = (struct parametri_thread_3 *)x;

    int valore;

    for (int i = 0; i < 3; i++)
    {

        valore = buffer_consuma(p->b2 /* TBD */);

        printf("[THREAD 3] Prelevato valore %d dal buffer 2\n", valore);

        sleep(1);

        valore = valore + 1;

        printf("[THREAD 3] Valore finale: %d\n", valore);
    }

    pthread_exit(NULL);
}
