#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "seller_buyer.h"


int main(){
    
    int i;
    
    pthread_t seller_threads[2];
    pthread_t buyer_threads[5];
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    
    srand(time(NULL));
    
    /* TBD: Aggiungere codice per l'allocazione della struttura LettScritt */
    
    m->quotazione_oro = 0;
    m->quotazione_argento = 0;
   
    /* TBD: Aggiungere codice per l'inizializzazione della variabili in LettScritt utilizzate per la sincronizzazione */
    
    
    for(i=0;i<2;i++){
         /* TBD: creazione dei threads seller */
    }
    
    
    for(i=0;i<5;i++){
        /* TBD: creazione dei threads buyer */
    }
    
    
    for(i=0;i<2;i++){
        /* TBD: Join dei threads seller */
        printf("seller_threads %d terminato\n",i+1);
    }
    
    
    for(i=0;i<5;i++){
        /* TBD: Join dei threads seller */
        printf("buyer_threads %d terminato\n",i+1);
    }
    
    
    /* TBD: deallocazione monitor e relative variabili usate per la sincronizzazione */
    
    pthread_exit(NULL);
}

