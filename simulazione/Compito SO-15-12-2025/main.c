#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include "seller_buyer.h"

int main(){

    pid_t pid;
    int status, k;
    
    //Creazione code tra Buyer e Report
    int ds_queue_buyer_control_oro, ds_queue_buyer_control_argento;

    //create queues
    int key_queue_buyer_control_oro = /* TBD: generare la chiave */
    int key_queue_buyer_control_argento = /* TBD: generare la chiave */
    
    ds_queue_buyer_control_oro = /* TBD: creare la coda per la quotazione oro */
    ds_queue_buyer_control_argento = /* TBD: generare la chiave per la quotazione argento */
    
    printf("[master] Coda ds_queue_buyer_control_oro creata! ID: %d\n", ds_queue_buyer_control_oro);
    printf("[master] Coda ds_queue_buyer_control_argento creata! ID: %d\n", ds_queue_buyer_control_argento);
    
    
    /* TBD: Aggiungere codice per lanciare l'eseguibile 'seller_buyer'. Utilizzare una
     * delle chiamate alla famiglia delle exec()
     */
    
    /* TBD: Aggiungere codice per lanciare l'eseguibile 'report'. Utilizzare una
     * delle chiamate alla famiglia delle exec()
     */
    
    for (k=0; k<2; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("Figlio n.ro %d e\' morto con status= %d \n ", pid, status);
    }
    
    return 0;
}
