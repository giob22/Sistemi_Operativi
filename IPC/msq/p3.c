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

    printf("PROCESSO P3 AVVIATO\n");
    printf("PROCESSO P1 AVVIATO\n");

    key_t queue_key = ftok("./start", 'a'); // perché P1, P2, P3 sono eseguibili separati
    int id_queue = msgget(queue_key, IPC_CREAT | 0664);
    
    
    if (id_queue > 0)
    {
        printf("[P3] DESCRITTORE della coda: %d\n", id_queue);
    }else
    {
        LOG(" id_queue: %d", id_queue);
        exit(-1);
    }
    float valori_p2;
    float valori_p1 = valori_p2 = 0;
    struct msg_calc messaggio_ricevuto;
    for (int i = 0; i < 22; i++)
    {
        // receive bloccante su tutti i messaggi disponibili sulla coda
        msgrcv(id_queue, &messaggio_ricevuto, sizeof(struct msg_calc)- sizeof(long), 0, 0); // ricevo solo i messaggi inviati da P1
        printf("[P3] messaggio ricevuto tipo %ld numero %f\n", 
            messaggio_ricevuto.type, 
            messaggio_ricevuto.numero);
        
        if (messaggio_ricevuto.type == P1)
        {
            valori_p1 += messaggio_ricevuto.numero;
        }else if (messaggio_ricevuto.type == P2)
        {
            valori_p2 += messaggio_ricevuto.numero;
        }
        
        
        // if tipo == 1 ... media per P1
        // else if tipo == 2 ... media per P2
    }

    printf("la media dei valori ricevuti da P1: %f\n", valori_p1/ (float)(11));
    printf("la media dei valori ricevuti da P2: %f\n", valori_p2/ (float)(11));
    


    return 0;
}