#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "seller_buyer.h"

int main(){
    
    //Creazione code tra Buyer e Report
    int ds_queue_buyer_control_oro, ds_queue_buyer_control_argento;
    
    //create queues
    int key_queue_buyer_control_oro = ftok("./main.c", 'a');/* TBD: generare la chiave */
    int key_queue_buyer_control_argento = ftok("./main.c", 'b');/* TBD: generare la chiave */
    
    /* TBD: ottenere il descrittore della coda per la quotazione oro */
    ds_queue_buyer_control_oro = msgget(key_queue_buyer_control_oro, 0);
    /* TBD: ottenere il descrittore della coda per la quotazione argento */
    ds_queue_buyer_control_argento = msgget(key_queue_buyer_control_argento, 0);
    
    printf("[report] Coda ds_queue_buyer_control_oro ID: %d\n", ds_queue_buyer_control_oro);
    printf("[report] Coda ds_queue_buyer_control_argento ID: %d\n", ds_queue_buyer_control_argento);
    
    Msg_Quotazione m_oro;
    Msg_Quotazione m_argento;
    
    int quotazione_oro_sum = 0;
    int quotazione_argento_sum = 0;
    double quotazione_oro_mean = 0;
    double quotazione_argento_mean = 0;
    
    int i, ret;
    
    for (i=0; i<10; i++){
        
        ret = msgrcv(ds_queue_buyer_control_oro, &m_oro, sizeof(Msg_Quotazione) - sizeof(long), 0, 0);
        if (ret<0){
            perror("Errore rcv ds_queue_buyer_control_oro");
            exit(-1);
        }
        quotazione_oro_sum += m_oro.quotazione;
    }
    
    for (i=0; i<10; i++){
        
        ret = msgrcv(ds_queue_buyer_control_argento, &m_argento, sizeof(Msg_Quotazione) - sizeof(long), 0, 0);
        if (ret<0){
            perror("Errore rcv ds_queue_buyer_control_argento");
            exit(-1);
        }
        quotazione_argento_sum += m_argento.quotazione;
    }
    
    quotazione_oro_mean = quotazione_oro_sum/10;
    quotazione_argento_mean = quotazione_argento_sum/10;
    
    printf("MEDIA QUOTAZIONE ORO: %.2f\n", quotazione_oro_mean);
    printf("MEDIA QUOTAZIONE ARGENTO: %.2f\n", quotazione_argento_mean);
    
    return 0;
}


