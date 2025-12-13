#ifndef _HEADER_MSG_
#define _HEADER_MSG_
#include <sys/types.h>

#define MSG_TYPE 1

typedef struct {

    /* TBD: Definire il messaggio di richiesta */
    long pid;
    int valori;

} msg_init_request;

typedef struct {

    /* TBD: Definire il messaggio di risposta */
    long type;
    int id_sem_invio;
    int id_shm_invio;

    int id_sem_ricezione;
    int id_shm_ricezione;
    


} msg_init_response;


#endif
