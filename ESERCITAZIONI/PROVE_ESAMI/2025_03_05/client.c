#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "header_msg.h"

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

    srand(getpid());

    for (int i = 0; i < 3; i++)
    {

        struct richiesta r;
        struct rts rts;
        struct ots ots;

        /* TBD: Inviare il messaggio "request to send" */
        rts.type = RTS;
        msgsnd(request, &rts, sizeof(struct rts) - sizeof(long), 0);

        printf("[CLIENT] Inviato messaggio RTS\n");

        // attendo la ricezione di un OTS
        msgrcv(ok_to_send, &ots, sizeof(struct ots) - sizeof(long), OTS, 0);

        /* TBD: Ricevere il messaggio "ok to send" */

        printf("[CLIENT] Ricevuto messaggio OTS\n");

        int valore = rand() % 10;

        r.type = TYPE;
        r.valore = valore;

        /* TBD: Inviare un messaggio con il valore */
        msgsnd(msqid, &r, sizeof(struct richiesta) - sizeof(long), 0);

        printf("[CLIENT] Inviato messaggio MSG con valore %d\n", valore);
    }
}