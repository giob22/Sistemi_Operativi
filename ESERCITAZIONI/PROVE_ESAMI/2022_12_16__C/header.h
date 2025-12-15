#ifndef _HEADER_H_
#define _HEADER_H_

#include <pthread.h>

#define TIPO 1
#define LETTORI 3

#define FTOK_PATH "."
#define FTOK_CHAR 'a'

typedef struct {

    // TODO: completare campi messaggio
    long type;
    int value;
    
    } Messaggio;

typedef struct {

    // TODO: completare inserendo i campi necessari a gestire il problema dei lettori-scrittori (unico scrittore)
    int buffer;

    int num_lettori;
    int num_scrittori;


    pthread_mutex_t mutex;
    pthread_cond_t cv_scritt;
    pthread_cond_t cv_lett;
    } MonitorLS;

#endif
