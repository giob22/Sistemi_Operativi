#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"


int main() {

    srand(getpid());


    /* TBD: Ottenere gli identificativi delle code di messaggi */

    key_t key_ots = ftok("./start.c", 'o');
    key_t key_rts = ftok("./start.c", 'r');
    key_t key_m = ftok("./start.c", 'm');

    int queue_rts = msgget(key_rts, 0);
    int queue_ots = msgget(key_ots, 0); 
    int queue_msg = msgget(key_m, 0);


    for(int i=0; i<NUM_RICHIESTE; i++) {

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
        if (ret < 0) {
            perror("Errore nell'invio di rts");
            exit(2);
        }
        ret = msgrcv(queue_ots, &ots, sizeof(ok_to_send) - sizeof(long), OTS, 0);
        if (ret < 0) {
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