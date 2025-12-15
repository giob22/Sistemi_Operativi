#ifndef _HEADER_PRODCONS
#define _HEADER_PRODCONS

#include <pthread.h>


#define VALIDO 0
#define NON_VALIDO 1

struct buffer {
    
    int valore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */
    int stato;

    pthread_mutex_t mutex;
    pthread_cond_t cv_cons;
    pthread_cond_t cv_prod;
};

void buffer_init(struct buffer * b);
void buffer_produci(struct buffer * b, int valore);
int buffer_consuma(struct buffer * b);
void buffer_destroy(struct buffer * b);

#endif