#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "header_msg.h"

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
    


    srand(getpid());

    for(int i=0; i<3; i++) {

        struct richiesta r;
        struct rts rts;
        struct ots ots;

        /* TBD: Inviare il messaggio "request to send" */
        rts.type = REQUEST_TO_SEND;
        int ret = msgsnd(msqid_rts, &rts, sizeof(struct rts) - sizeof(long), 0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }
        printf("[CLIENT] Inviato messaggio RTS\n");



        
        /* TBD: Ricevere il messaggio "ok to send" */
        ret = msgrcv(msqid_ots, &ots, sizeof(struct ots)- sizeof(long), OK_TO_SEND, 0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }
        printf("[CLIENT] Ricevuto messaggio OTS\n");


        int valore = rand() % 10;
        r.type = MESSAGE;
        r.value = valore;
        /* TBD: Inviare un messaggio con il valore */
        ret = msgsnd(msqid_req, &r, sizeof(struct richiesta) - sizeof(long), 0);
        if (ret < 0)
        {
            perror("Errore");
            exit(3);
        }
        printf("[CLIENT] Inviato messaggio MSG con valore %d\n", valore);

    }
}