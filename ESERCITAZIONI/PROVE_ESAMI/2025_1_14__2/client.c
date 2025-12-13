#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>


#include "header.h"

int main() {

    /* TBD: Ottenere i riferimenti alle code di messaggi */

    key_t k_queue_req_1 = ftok("./start.c", 'a'); 
    key_t k_queue_res_1 = ftok("./start.c", 'b');
    

    int msqid_req_1 = msgget(k_queue_req_1, IPC_CREAT | 0664); 
    int msqid_res_1 = msgget(k_queue_res_1, IPC_CREAT | 0664);
    

    srand(getpid());

    for(int i=0; i<NUMERO_RICHIESTE; i++) {
        

        int tipo_richiesta;
        int valore;
        int posizione = rand() % DIM_VETTORE;

        if((rand() % 2) == 0) {

            tipo_richiesta = LETTURA;
            valore = 0;
        }
        else {
            
            tipo_richiesta = SCRITTURA;
            valore = rand() % 10;
        }

        printf("[CLIENT %d] Invio richiesta (tipo=%s, valore=%d, posizione=%d)\n", getpid(), (tipo_richiesta == LETTURA) ? "LETTURA" : "SCRITTURA", valore, posizione);

        /* TBD: Inviare un messaggio di richiesta */

        messaggio_richiesta req;

        req.type = getpid();
        req.pos = posizione;
        req.valore =valore;
        req.op = tipo_richiesta;

        msgsnd(msqid_req_1, &req, sizeof(messaggio_richiesta) - sizeof(long), 0);



        
        /* TBD: Ricevere il messaggio di risposta */

        messaggio_risposta res;
        msgrcv(msqid_res_1, &res, sizeof(messaggio_risposta) - sizeof(long), getpid(), 0);
        

        int valore_risposta = /* TBD */res.valore;

        printf("[CLIENT %d] Ricezione risposta (valore=%d)\n", getpid(), valore_risposta);
    }


}