#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"


void collettore(int id_queue_collettore, int id_queue_server) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        messaggio_sensore sensore;
        for(int j=0; j<NUM_SENSORI; j++) {
            //TODO: riceve un messaggio da ogni sensore
            msgrcv(id_queue_collettore, &sensore, sizeof(messaggio_sensore) - sizeof(long), j + 1, 0);

            int valore = sensore.val;//TODO;

            printf("[COLLETTORE] Ricevuto valore=%d dal sensore %d\n", valore, j+1);

            somma_valori += valore;
        }


        media = somma_valori/NUM_SENSORI;//TODO: calcolo media

        //TODO manda la media al server
        messaggio_collettore c;
        c.type = TYPE_COLLETTORE;
        c.val = media;
        msgsnd(id_queue_server, &c, sizeof(messaggio_collettore)- sizeof(long), 0);

        

        printf("[COLLETTORE] Inviato valore=%d\n", media);


    }
}
