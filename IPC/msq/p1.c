#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "header.h"

#define P1 1
#define P2 2


int main(){

    printf("PROCESSO P1 AVVIATO\n");

        key_t queue_key = ftok("./start", 'a'); // perché P1, P2, P3 sono eseguibili separati
        // questo significa che non posso utilizzare una coda privata
        // perché i figli non avranno accesso al descrittore della coda
        // non è un buon metodo nemmeno utilizzare una chiave cablata
        // perché un'altro processo potrebbe utilizzare la stessa chiave 
        // per altre risorse, come anche altre message queue.
    int id_queue = msgget(queue_key, IPC_CREAT | 0664);
    
    
    if (id_queue > 0)
    {
        printf("[P1] DESCRITTORE della coda: %d\n", id_queue);
    }else
    {
        LOG(" id_queue: %d", id_queue);
        exit(-1);
    }

    srand(time(NULL) * getpid());

    struct msg_calc messaggio;

    //messaggio.type = getpid();
    messaggio.type = P1; //P1 = 1 P2 = 2 // sono due define


    for (int i = 0; i < 11; i++)
    {
        float valore = generaFloat(0,10);
        // mandare il messaggio
        // costruisco prima il messaggio
        messaggio.numero = valore;

        printf("[P1] INVIO messaggio type: %ld e numero: %f\n", messaggio.type, messaggio.numero);
        // send asincrona, perché non abbiamo richieste particolari
        // faccio che sia anche bloccante
        msgsnd(id_queue, &messaggio, sizeof(struct msg_calc) - sizeof(long), 0);
    }
    


    return 0;
}