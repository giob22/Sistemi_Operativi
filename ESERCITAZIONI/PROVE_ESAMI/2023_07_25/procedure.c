#include <stdio.h>

#include "procedure.h"

void inizializza(MonitorPortafoglio * m, int quantita_iniziale, int valore_iniziale) {

    /* TBD: Inizializzare l'oggetto monitor */
    pthread_mutex_init(&(m->mutex), NULL);
    pthread_cond_init(&(m->cv_scritt), NULL);
    pthread_cond_init(&(m->cv_lett), NULL);

    m->num_scrittori = 0;
    m->num_scrittori_s = 0;
    m->num_lettori = 0;

    m->quantita = quantita_iniziale;
    m->valore = valore_iniziale;
}

void termina(MonitorPortafoglio * m) {

    /* TBD: De-inizializzare l'oggetto monitor */
    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->cv_scritt));
    pthread_cond_destroy(&(m->cv_lett));

}

void acquisto(MonitorPortafoglio * m, int quantita, int prezzo) {

    /* TBD: Aggiungere la sincronizzazione */
    // entra nel monitor
    pthread_mutex_lock(&(m->mutex));
    
    // verifica la condizione di sincronizzazione
    while (m->num_lettori > 0 || m->num_scrittori > 0 || prezzo < m->valore){
        m->num_scrittori_s++;
        pthread_cond_wait(&(m->cv_scritt), &(m->mutex));
        m->num_scrittori_s--;
    } 
    
    m->num_scrittori++;
    
    pthread_mutex_unlock(&(m->mutex));
    // aggiorna la quantità a una condizione
    
    m->quantita += quantita;

    pthread_mutex_lock(&(m->mutex));
    
    m->num_scrittori--;
    
    if (m->num_scrittori_s > 0)
    pthread_cond_signal(&(m->cv_scritt));
    else
    pthread_cond_broadcast(&(m->cv_lett));
    
    pthread_mutex_unlock(&(m->mutex));
}

void vendita(MonitorPortafoglio * m, int quantita, int prezzo) {
    
    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&(m->mutex));

    // condizione di sincronizzazione
    while (m->num_lettori > 0 || m->num_scrittori > 0 || prezzo > m->valore || quantita > m->quantita){
        m->num_scrittori_s++;
        pthread_cond_wait(&(m->cv_scritt), &(m->mutex));
        m->num_scrittori_s--;
    }

    m->num_scrittori++;
    pthread_mutex_unlock(&(m->mutex));

    m->quantita -= quantita;

    pthread_mutex_lock(&(m->mutex));

    m->num_scrittori--;

    if (m->num_scrittori_s > 0)
        pthread_cond_signal(&(m->cv_scritt));
    else
        pthread_cond_broadcast(&(m->cv_lett));

    pthread_mutex_unlock(&(m->mutex));

}

void aggiorna(MonitorPortafoglio * m, int valore) {

    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&(m->mutex));

    // condizione di sincronizzazione
    while (m->num_lettori > 0 || m->num_scrittori > 0){
        m->num_scrittori_s++;
        pthread_cond_wait(&(m->cv_scritt), &(m->mutex));
        m->num_scrittori_s--;
    }

    m->num_scrittori++;

    pthread_mutex_unlock(&(m->mutex));

    m->valore = valore;
    // valore aggiornato

    pthread_mutex_lock(&(m->mutex));

    m->num_scrittori--;

    if (m->num_scrittori_s > 0)
        pthread_cond_signal(&(m->cv_scritt));
    else
        pthread_cond_broadcast(&(m->cv_lett));

    pthread_mutex_unlock(&(m->mutex));
}

int leggi(MonitorPortafoglio * m) {

    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&(m->mutex));

    // condizione di sincronizzazione per i lettori
    while (m->num_scrittori > 0)
        pthread_cond_wait(&(m->cv_lett), &(m->mutex));

    m->num_lettori++;

    pthread_mutex_unlock(&(m->mutex));

    int valore;

    valore = m->valore;

    pthread_mutex_lock(&(m->mutex));
    m->num_lettori--;
    // controlla se è l'ultimo lettore
    if (m->num_lettori == 0)
        pthread_cond_signal(&(m->cv_scritt));
    
    pthread_cond_unlock(&(m->mutex));
    
    return valore;
}