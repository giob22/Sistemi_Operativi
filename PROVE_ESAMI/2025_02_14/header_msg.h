#ifndef _HEADER_
#define _HEADER_


#define TYPE_COLLETTORE 45

typedef struct {

    //TODO completare struttura messaggio
    long type;
    int val;

} messaggio_sensore;

typedef struct {

    //TODO completare struttura messaggio
    long type;
    int val;
} messaggio_collettore;


#define NUM_MESSAGGI_PER_SENSORE 5
#define NUM_SENSORI 3

void sensore(int id_sensore, int id_queue_collettore);
void collettore(int id_queue_collettore, int id_queue_server);

#endif