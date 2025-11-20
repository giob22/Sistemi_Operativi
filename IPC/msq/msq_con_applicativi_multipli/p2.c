#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "header.h"

int main(){

    printf("PROCESSO P2 AVVIATO\n");

    key_t queue_key = ftok(".", 'a'); // perché P1, P2, P3 sono eseguibili separati
    int id_queue = msgget(queue_key, IPC_CREAT | 0664);
    
    
    if (id_queue > 0)
    {
        printf("[P2] DESCRITTORE della coda: %d\n", id_queue);
    }else
    {
        LOG(" id_queue: %d", id_queue);
        exit(-1);
    }

        struct msg_calc messaggio;

    //messaggio.type = getpid();
    messaggio.type = P2; //P1 = 1 P2 = 2 // sono due define


    for (int i = 0; i < 11; i++)
    {
        float valore = generaFloat(0,10);
        // mandare il messaggio
        // costruisco prima il messaggio
        messaggio.numero = valore;

        printf("[P2] INVIO messaggio type: %ld e numero: %f\n", messaggio.type, messaggio.numero);
        // send asincrona, perché non abbiamo richieste particolari
        // faccio che sia anche bloccante
        msgsnd(id_queue, &messaggio, sizeof(struct msg_calc) - sizeof(long), 0);
    }

    return 0;
}