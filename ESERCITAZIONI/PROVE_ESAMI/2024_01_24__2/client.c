#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#include "header_msg.h"
#include "header_sem.h"

int main()
{

    srand(getpid());

    /* TBD: Ottenere gli identificativi delle code di messaggi */

    key_t key_ots = ftok("./start.c", 'o');
    key_t key_rts = ftok("./start.c", 'r');
    key_t key_m = ftok("./start.c", 'm');

    /*int queue_rts = msgget(key_rts, 0);
    if (queue_rts < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: RTS queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_rts");
            exit(1);
        }
    }*/
    int queue_rts = msgget(key_rts, IPC_CREAT | IPC_EXCL | 0664); // prova a creare la coda in esclusiva
    if (queue_rts < 0)
    {
        // la coda è stata già creata, msgget ha restituito -1
        // ottengo solamente il descrittore della coda 
        /*if (errno == EEXIST)
        {
            printf("La coda già esiste\n");
        }*/
        queue_rts = msgget(key_rts, 0);
    }else{
        // errno = EEXIST, ovvero la coda già esiste
        // però ha creato una coda la prima chiamata, quindi devo eliminarla
        msgctl(queue_rts, IPC_RMID, NULL);
        printf("%d\n", queue_rts);
        perror("msgget queue_rts");
        exit(1);
    }
    

    int queue_ots = msgget(key_ots, 0);
    if (queue_ots < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: OTS queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_ots");
            exit(1);
        }
    }

    int queue_msg = msgget(key_m, 0);
    if (queue_msg < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: MSG queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_msg");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_RICHIESTE; i++)
    {

        int buffer = rand() % NUM_BUFFER;

        int value = rand() % 10;

        /* TBD: Inviare un messaggio contenente "value" e "buffer".

                È richiesto che l'invio sia effettuato
                tramite send sincrona.
         */
        ok_to_send ots;
        request_to_send rts;
        Msg msg;
        rts.type = RTS;
        int ret;
        ret = msgsnd(queue_rts, &rts, sizeof(request_to_send) - sizeof(long), 0);
        if (ret < 0)
        {
            perror("Errore nell'invio di rts");
            exit(2);
        }
        ret = msgrcv(queue_ots, &ots, sizeof(ok_to_send) - sizeof(long), OTS, 0);
        if (ret < 0)
        {
            perror("Errore nella ricezione di ots");
            exit(2);
        }
        msg.type = TYPE_MSG;
        msg.id_buff = buffer;
        msg.val = value;
        ret = msgsnd(queue_msg, &msg, sizeof(Msg) - sizeof(long), 0);

        sleep(2);
    }
}