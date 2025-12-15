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

void * seller(void *m){
    
    LettScritt * ls = (LettScritt*)m;
    
    int i, quotazione;
    
    for(i=0; i<10; i++) {
        
        if (i%2 == 0){
            quotazione = rand() % 1000 + 1000;
            scrivi_quotazione_oro(ls, quotazione);
            
        }else{
            quotazione = rand() % 500 + 1000;
            scrivi_quotazione_argento(ls, quotazione);
            
        }
        
    }
    
    pthread_exit(0);
}

void * buyer(void *m){
    
    LettScritt * ls = (LettScritt*)m;
    
    int i;
    
    int ds_queue_buyer_control_oro, ds_queue_buyer_control_argento;
    
    //create queues
    int key_queue_buyer_control_oro = ftok("./main.c", 'a');/* TBD: generare la chiave */
    int key_queue_buyer_control_argento = ftok("./main.c", 'b');/* TBD: generare la chiave */
    
    /* TBD: ottenere il descrittore della coda per la quotazione oro */
    ds_queue_buyer_control_oro = msgget(key_queue_buyer_control_oro, 0);
    /* TBD: ottenere il descrittore della coda per la quotazione argento */
    ds_queue_buyer_control_argento = msgget(key_queue_buyer_control_argento, 0);
    
    printf("[buyer] Coda ds_queue_buyer_control_oro ID: %d\n", ds_queue_buyer_control_oro);
    printf("[buyer] Coda ds_queue_buyer_control_argento ID: %d\n", ds_queue_buyer_control_argento);
    
    for(i=0; i<4; i++) {
        
        if (i%2 == 0){
            leggi_quotazione_oro(ls, ds_queue_buyer_control_oro);
            sleep(2);
            
        }else{
            leggi_quotazione_argento(ls, ds_queue_buyer_control_argento);
            sleep(2);
        }
        
    }
    
    pthread_exit(0);
    
}

void scrivi_quotazione_oro(LettScritt *ls, int quotazione){
    
    InizioScritturaQuotazione(ls);
    
    ls->quotazione_oro = quotazione;
    
    printf("Thread ORO - Quotazione SCRITTO = %d \n", ls->quotazione_oro);
    
    FineScritturaQuotazione(ls);
}

void scrivi_quotazione_argento(LettScritt *ls, int quotazione){
    
    InizioScritturaQuotazione(ls);
    
    ls->quotazione_argento = quotazione;
    
    printf("Thread ARGENTO - Quotazione SCRITTO = %d \n", ls->quotazione_argento);
    
    FineScritturaQuotazione(ls);
}


void leggi_quotazione_oro(LettScritt *ls, int ds_queue_buyer_control_oro){
    
    InizioLetturaQuotazione(ls);
    
    printf("Thread ORO - Quotazione LETTA = %d \n", ls->quotazione_oro);

    //invio messaggio a report
    Msg_Quotazione quotazione;
    int ret;
    
    /* TBD: Aggiungere codice per l'invio della quotazione oro al processo Report
     */
    quotazione.quotazione = ls->quotazione_oro;
    quotazione.type = ORO;
    ret = msgsnd(ds_queue_buyer_control_oro, &quotazione, sizeof(Msg_Quotazione) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore nell'invio del messaggio");
        exit(-2);
    }

    FineLetturaQuotazione(ls);
    
}

void leggi_quotazione_argento(LettScritt *ls, int ds_queue_buyer_control_argento){
    
    InizioLetturaQuotazione(ls);
    
    printf("Thread ARGENTO - Quotazione LETTA = %d \n", ls->quotazione_argento);
    
    //invio messaggio a report
    Msg_Quotazione quotazione;
    int ret;
    
    /* TBD: Aggiungere codice per l'invio della quotazione argento al processo Report
     */
    quotazione.quotazione = ls->quotazione_argento;
    quotazione.type = ARGENTO;
    ret = msgsnd(ds_queue_buyer_control_argento, &quotazione, sizeof(Msg_Quotazione) - sizeof(long), 0);
    if (ret < 0) {
        perror("Errore nell'invio del messaggio");
        exit(-2);
    }
    
    FineLetturaQuotazione(ls);
}

/* Metodi privati del monitor */

void InizioLetturaQuotazione(LettScritt * ls){
    
    /* TBD: Aggiungere codice per l'inizio lettura in accordo al problema
     * lettori-scrittori con starvation di entrambi
     */
    pthread_mutex_lock(&(ls->mutex));

    // condizione di sincronizzazione
    while (ls->num_scrittori > 0) {
        pthread_cond_wait(&(ls->cv_lett), &(ls->mutex));
    }
    ls->num_lettori++;
    pthread_mutex_unlock(&(ls->mutex));
}

void FineLetturaQuotazione(LettScritt * ls){
    
    /* TBD: Aggiungere codice per la fine lettura in accordo al problema
     * lettori-scrittori con starvation di entrambi
     */

    pthread_mutex_lock(&(ls->mutex));

    ls->num_lettori--;
    if (ls->num_lettori == 0) {
        pthread_cond_signal(&(ls->cv_scritt));
    }

    pthread_mutex_unlock(&(ls->mutex));
}


void InizioScritturaQuotazione(LettScritt * ls){
    
    /* TBD: Aggiungere codice per l'inizio scrittura in accordo al problema
     * lettori-scrittori con starvation di entrambi
     */
    
    pthread_mutex_lock(&(ls->mutex));

    // condizione di sincronizzazione
    while (ls->num_lettori > 0 || ls->num_scrittori > 0){
        ls->num_scrittori_s++;
        pthread_cond_wait(&(ls->cv_scritt), &(ls->mutex));
        ls->num_scrittori_s--;
    }
    ls->num_scrittori++;
    pthread_mutex_unlock(&(ls->mutex));
}

void FineScritturaQuotazione (LettScritt * ls){
    
    /* TBD: Aggiungere codice per la fine scrittura in accordo al problema
     * lettori-scrittori con starvation di entrambi
     */
    pthread_mutex_lock(&(ls->mutex));
    ls->num_scrittori--;

    // verfica quale tipologia di thread andare a risvegliare
    if (ls->num_scrittori_s > 0) {
        pthread_cond_signal(&(ls->cv_scritt));
    }else {
        pthread_cond_broadcast(&(ls->cv_lett));
    }
    pthread_mutex_unlock(&(ls->mutex));


    
}
