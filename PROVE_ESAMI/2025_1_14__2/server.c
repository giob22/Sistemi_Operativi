#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main() {

    /* TBD: Ottenere i riferimenti alle code di messaggi */

    key_t k_queue_req_1 = ftok("./start.c", 'a'); 
    key_t k_queue_res_1 = ftok("./start.c", 'b');
    key_t k_queue_req_2 = ftok("./start.c", 'c');
    key_t k_queue_res_2 = ftok("./start.c", 'd');

    int msqid_req_1 = msgget(k_queue_req_1, IPC_CREAT | 0664); 
    int msqid_res_1 = msgget(k_queue_res_1, IPC_CREAT | 0664);
    int msqid_req_2 = msgget(k_queue_req_2, IPC_CREAT | 0664);
    int msqid_res_2 = msgget(k_queue_res_2, IPC_CREAT | 0664);


    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta dal client */

        messaggio_richiesta req;

        msgrcv(msqid_req_1, &req, sizeof(messaggio_richiesta) - sizeof(long), 0, 0);

        int tipo_richiesta = /* TBD */req.op;
        int valore = /* TBD */req.valore;
        int posizione = /* TBD */req.pos;
        int pid_client = /* TBD */req.type;

        printf("[SERVER %d] Inoltro richiesta (tipo=%s, pid=%d, posizione=%d, valore=%d)\n", getpid(), (tipo_richiesta == LETTURA) ? "LETTURA" : "SCRITTURA", pid_client, posizione, valore);

        sleep(1);


        /* TBD: Inviare un messaggio di richiesta al database, contenente lo stesso tipo di richiesta (LETTURA/SCRITTURA), posizione, e valore */

        messaggio_richiesta req_to_database;
        req_to_database.type = getpid();
        req_to_database.op = tipo_richiesta;
        req_to_database.pos = posizione;
        req_to_database.valore = valore;

        msgsnd(msqid_req_2, &req_to_database, sizeof(messaggio_richiesta) - sizeof(long), 0);


        /* TBD: Ricevere un messaggio di risposta dal database */
        messaggio_risposta res_from_database;
        msgrcv(msqid_res_2, &res_from_database, sizeof(messaggio_risposta) - sizeof(long), getpid(), 0);

        int valore_risposta = res_from_database.valore/* TBD */;

        printf("[SERVER %d] Inoltro risposta (pid=%d, valore=%d)\n", getpid(), pid_client, valore_risposta);



        /* TBD: Inviare il messaggio di risposta al client, contenente lo stesso valore risposta */
        messaggio_risposta res_for_client;
        res_for_client.valore = valore_risposta;
        res_for_client.type = pid_client;

        msgsnd(msqid_res_1, &res_for_client, sizeof(messaggio_risposta) - sizeof(long), 0);


    }

}