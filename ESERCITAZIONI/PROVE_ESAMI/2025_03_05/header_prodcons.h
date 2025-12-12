#ifndef _HEADER_PRODCONS
#define _HEADER_PRODCONS

#include <pthread.h>

#define VALIDO 1
#define NON_VALIDO 0

struct buffer {
    
    int valore;

    int valido; // indica se il dato è presente o meno


    pthread_mutex_t mutex;
    pthread_cond_t cv_cons;
    pthread_cond_t cv_prod;
    /* TBD: Aggiungere altre variabili per la sincronizzazione */
};

void buffer_init(struct buffer * b);
void buffer_produci(struct buffer * b, int valore);
int buffer_consuma(struct buffer * b);
void buffer_destroy(struct buffer * b);

#endif