#include "header.h"


void* produttore(void* p){
    struct prodcons* pc = (struct prodcons*)p;
    // ogni produttore produce 4 volte un valore
    for (int i = 0; i < NUM_VALORI; i++){
        produci(pc);
    }
    
    #ifdef DEBUG
    LOG("fine produzione");
    #endif
    
    
    pthread_exit(NULL);
}
void* consumatore(void* p){
    struct prodcons* pc = (struct prodcons*)p;
    printf("consumatore avviato\n");
    for (int i = 0; i < NUM_VALORI; i++){
        consuma(pc);
    }
    #ifdef DEBUG
    LOG("fine consumazione");
    #endif
    pthread_exit(NULL);    
}


void produci(struct prodcons* pc){
    
    // prima di iniziare la consumazione entra all'interno del monitor
    
    pthread_mutex_lock(&(pc->mutex));	
    #ifdef DEBUG
        LOG("il produttore è nel monitor");
    #endif
        
        // condizione di sincronizzazione
        while (pc->stato == PIENO){ // allora il produttore non può produrre
            #ifdef DEBUG
            LOG("il produttore è in attesa");
            #endif
            pthread_cond_wait(&(pc->ok_produci_cond), &(pc->mutex));
            #ifdef DEBUG
                if (pc->stato == VUOTO)
                LOG("il produttore è stato sbloccato e rientra nel monitor");
            #endif
    }
    
    // nel momento in cui acquisisce il monitor e la possibilità di produrre
    
    int valore_prodotto = rand()%100;
    
    
    pc->buffer = valore_prodotto;
    // modifico le variabili di sincronizzazione
    pc->stato = PIENO;
    
    // esegue un signal_cond per risvegliare eventuali lettori che sono in attesa per la condizione di sincronizzazione
    #ifdef DEBUG
    LOG("il produttore fa una signal_cond per risvegliare eventuali consumatori");
    #endif
    pthread_cond_signal(&(pc->ok_consuma_cond));
    
    // una volta finita la produzione esce dal monitor
    
    #ifdef DEBUG
        LOG("il produttore esce dal monitor");
    #endif
    pthread_mutex_unlock(&(pc->mutex));
}
int consuma(struct prodcons* pc){
    // prima di consumare acquisisce il monitor
    pthread_mutex_lock(&(pc->mutex));
    #ifdef DEBUG
    LOG("il consumatore è nel monitor");
    #endif
    
    // verifica la condizione di sincronizzazione
    while (pc->stato == VUOTO){ // in questo caso il consumatore deve attendere
        #ifdef DEBUG
        LOG("il consumatore è in attesa");
        #endif
        pthread_cond_wait(&(pc->ok_consuma_cond),&(pc->mutex));
        #ifdef DEBUG
        if (pc->stato == PIENO)
        LOG("il consumatore è stato sbloccato e rientra nel monitor");
        #endif
    }
    
    // quando acquisisce il monitor e ha verificata la condizione di sincronizzazione → consuma
    int valore_consumato = pc->buffer;
    printf("[C] valore consumato: %d\n",valore_consumato);
    
    // modifica le variabili di sincronizzazione
    pc->stato = VUOTO;
    
    // esegue una signal per riscegliare eventuali produttori in attesa per la produzione
    #ifdef DEBUG
        LOG("il consumatore fa una signal_cond per risvegliare eventuali produttori");
    #endif
    pthread_cond_signal(&(pc->ok_produci_cond));
    
    // rilascia il monitor
    #ifdef DEBUG
        LOG("il consumatore esce dal monitor");
    #endif
    pthread_mutex_unlock(&(pc->mutex));
}
