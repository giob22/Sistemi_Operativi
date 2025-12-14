#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

void inizializza_coda(MonitorPC *pc) {
/*
    TODO: completare
*/
    pc->coda = 0;
    pc->testa = 0;
    pc->dim = 0;
    init_monitor(&(pc->m),2);
}

void rimuovi_coda(MonitorPC *pc) {
/*
    TODO: completare
*/
    remove_monitor(&(pc->m));
}

void produci(MonitorPC *pc, Lettera lettera) {
/*
    TODO: completare produzione con coda circolare
*/
    enter_monitor(&(pc->m));

    // condizione di sincronizzazione
    while (pc->dim == DIM) {
        wait_condition(&(pc->m), CV_PROD);
    }
    pc->dim++;
    pc->buffer[pc->testa] = lettera;
    pc->testa = (pc->testa + 1)%DIM;

    printf("[Produttore %d] Ho inviato lettera: ID=%d, regali=%d\n", getpid(), lettera.id_bambino, lettera.numero_regali);

    signal_condition(&(pc->m), CV_CONS);

    leave_monitor(&(pc->m));
}

Lettera consuma(MonitorPC *pc) {
    Lettera lettera;
/*
    TODO: completare consumo con coda circolare
*/
    enter_monitor(&(pc->m));

    // condizione di sincronizzazione
    while (pc->dim == 0) {
        wait_condition(&(pc->m), CV_CONS);
    }
    pc->dim--;
    lettera = pc->buffer[pc->coda];
    pc->coda = (pc->coda + 1)%DIM;

    signal_condition(&(pc->m), CV_PROD);

    leave_monitor(&(pc->m));


    return lettera;
}

void SendSincr (MessaggioRispostaBabbo *mess, int queue){
/*
    TODO: completare
*/
    ok_to_send ots;
    request_to_send rts;

    // invio richiesta
    rts.type = RTS;
    int ret;
    ret = msgsnd(queue, &rts, sizeof(request_to_send) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore nell'invio di rts\n");
        exit(1);
    }

    // attende la risposta
    ret = msgrcv(queue, &ots, sizeof(ok_to_send) - sizeof(long), OTS, 0);


    if (ret < 0) {
        perror("Errore nella ricezione di ots\n");
        exit(1);
    }

    // invio del messaggio
    ret = msgsnd(queue, mess, sizeof(MessaggioRispostaBabbo) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore nell'invio di mess\n");
        exit(1);
    }

}

void ReceiveBloc(MessaggioRispostaBabbo *mess, int queue){
/*
    TODO: completare
*/
    request_to_send rts;
    ok_to_send ots;
    // attende un rts
    // attende la risposta
    
    int ret;
    ret = msgrcv(queue, &rts, sizeof(request_to_send) - sizeof(long), RTS, 0);

    if (ret < 0) {
        perror("Errore nella ricezione di rts\n");
        exit(1);
    }

    // invio del messaggio ots
    ots.type = OTS;
    ret = msgsnd(queue, &ots, sizeof(ok_to_send) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore nell'invio di ots\n");
        exit(1);
    }

    // riceve l'effettivo messaggio
    ret = msgrcv(queue, mess, sizeof(MessaggioRispostaBabbo) - sizeof(long), 0, 0);


    ///-----------------------------------------CONTROLLA I TYPE

}

void Produttore(MonitorPC *pc, int id_bambino, int coda_risposte) {
    /*
        TODO: completare inizializzazione lettera e produzione
    */
    Lettera lettera;

    lettera.id_bambino = id_bambino;
    lettera.numero_regali = rand()%5 + 1;

    
    produci(pc,lettera);
    /*
        TODO: attesa risposta
    */
    MessaggioRispostaBabbo risposta;
    ReceiveBloc(&risposta, coda_risposte);

    printf("[Produttore %d] Risposta ricevuta: %s\n", getpid(), risposta.messaggio);
}

void Consumatore(MonitorPC *pc, int coda_risposte) {
    int cattivi[] = {3, 5, 7, 9};
    int num_cattivi = sizeof(cattivi) / sizeof(cattivi[0]);

    for (int i=0; i<NUM_BAMBINI; i++) {
        /*
            TODO: completare consumo di una lettera
        */
        
        
        Lettera lettera;

        lettera = consuma(pc);
        

        printf("[Babbo Natale] Ho ricevuto una lettera con ID=%d e regali=%d\n", lettera.id_bambino, lettera.numero_regali);

        /*
            TODO: creazione messaggio di risposta, controllo dell'id e invio del messaggio di risposta
        */
        MessaggioRispostaBabbo risposta;
        controlla_buono(&risposta, lettera.id_bambino, cattivi);


        printf("[Babbo Natale] Il bambino è stato %s\n", risposta.messaggio);
        risposta.id_bambino = lettera.id_bambino;
        SendSincr(&risposta, coda_risposte);

        sleep(1);
    }
}

void controlla_buono(MessaggioRispostaBabbo *risposta, int id, int *cattivi) {
    strcpy(risposta->messaggio, "buono");
    for (int i = 0; i < NUM_CATTIVI; i++) {
        if (cattivi[i] == id) {
            strcpy(risposta->messaggio, "cattivo");
            break; 
        }
    }
}

