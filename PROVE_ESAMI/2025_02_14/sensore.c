#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"

void sensore(int id_sensore, int id_queue_collettore) {

    srand(getpid());

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        messaggio_sensore msg;

        int valore = (rand() % 10) + 10;

        msg.type = id_sensore;
        msg.val = valore;
        //TODO: invio del valore al collettore
        msgsnd(id_queue_collettore, &msg, sizeof(messaggio_sensore) - sizeof(long), 0);
    

        printf("[SENSORE] Inviato valore=%d\n", valore);


        sleep(1);
    }
}