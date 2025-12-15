#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

/* TBD: Definire le macro necessarie per le code di messaggio */

#define ORO 24
#define ARGENTO 44
 
typedef struct {
    
    /* TBD: Definire le variabili necessarie per la sincronizzazione */
    
    //Var condivise
    int quotazione_oro;
    int quotazione_argento;

    int num_lettori;
    int num_scrittori;
    int num_scrittori_s;

    pthread_mutex_t mutex;
    pthread_cond_t cv_lett;
    pthread_cond_t cv_scritt;
}LettScritt;

typedef struct {
    
    /* TBD: Definire i campi necessari per lo scambio di messaggi inerenti alla quotazione */
    int type;
    int quotazione;
    
} Msg_Quotazione;

void * seller(void *);
void * buyer(void *);

void scrivi_quotazione_oro(LettScritt *ls, int quotazione_oro);
void scrivi_quotazione_argento(LettScritt *ls, int quotazione_argento);
void leggi_quotazione_oro(LettScritt *ls, int);
void leggi_quotazione_argento(LettScritt *ls, int);

void InizioLetturaQuotazione(LettScritt * ls);
void FineLetturaQuotazione(LettScritt * ls);
void FineScritturaQuotazione(LettScritt * ls);
void InizioScritturaQuotazione(LettScritt * ls);


#endif
