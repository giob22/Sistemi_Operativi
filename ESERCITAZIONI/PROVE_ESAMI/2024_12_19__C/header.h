#ifndef HEADER_H
#define HEADER_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define NUM_LETTERE 5
#define NUM_BAMBINI 3

/*
    TODO: completare le strutture necessarie per lo scambio di messaggi
*/

typedef struct
{

    long type;
    int numero_regali;

} MessaggioRichiesta;

typedef struct
{

    long type;

} MessaggioRisposta;

typedef struct
{
    /*
        TODO: completare la struttura per risolvere il problema lettori-scrittori
    */
    int totale_regali;

    int num_lettori;
    int num_scrittori;
    int num_scrittori_s;

    pthread_mutex_t mutex;
    pthread_cond_t cv_lett;
    pthread_cond_t cv_scritt;

} MonitorLettScritt;

#endif