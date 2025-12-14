#include "monitor_signal_continue.h"

#define DIM 5
#define NUM_BAMBINI 10
#define NUM_CATTIVI 4

#define RTS 40
#define OTS 41
#define RISPOSTA 55


#define CV_PROD 0
#define CV_CONS 1

/*
    TODO: completare le strutture necessarie alla gestione della coda
*/

typedef struct {
/*
    TODO: completare 
*/
    int id_bambino;
    int numero_regali;
} Lettera;

typedef struct {
    Lettera buffer[DIM];
/*
    TODO: completare 
*/
    int testa,coda,dim;

    Monitor m;
} MonitorPC;

/*
    TODO: completare le strutture per la gestione dello scambio di messaggi con send sincrona
*/

typedef struct {
    long type;
} ok_to_send;
typedef struct {
    long type;
} request_to_send;
typedef char msg[20];

typedef struct {
    long id_bambino;
    msg messaggio;
} MessaggioRispostaBabbo;

void inizializza_coda(MonitorPC *m);
void rimuovi_coda(MonitorPC *m);
void produci(MonitorPC *m, Lettera lettera);
Lettera consuma(MonitorPC *m);
void controlla_buono(MessaggioRispostaBabbo * risposta, int id, int *cattivi);

void ReceiveBloc (MessaggioRispostaBabbo *, int);
void SendSincr (MessaggioRispostaBabbo *, int);

void Produttore(MonitorPC *m, int id, int coda_risposte);
void Consumatore(MonitorPC *m, int coda_risposte);
