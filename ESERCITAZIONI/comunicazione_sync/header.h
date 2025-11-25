// devo implementare una comunicazione sincrona tra processi produttori e processi scrittori
// utilizzando le message queue fornite da System V
// Devo implementare una send sincrona mediante lo schema RENDEZVOUS

#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define LOGC(stringa, ...) printf("[CONSUMATORE] " stringa "\n", ##__VA_ARGS__)
#define LOGP(stringa, ...) printf("[PRODUTTORE] " stringa "\n", ##__VA_ARGS__)


// tipi di messaggi necessari
#define RTS 1
#define RTR 2
#define TYPE 3

// definisco la struttura del messaggio

typedef char msg[40];

typedef struct {
	long type;

	// singolo buffer → ipotizziamo sia un intero
	msg text;
} message;
// messaggio che utilizzo solo per la sincronizzazione
// contenente unicamente il tipo che sarà intestazione + payload
typedef struct {
	long type;
} synch_msg;


void init_queue(int*);

void receive_bloccante(message*, int, int);
void send_sincrona(message*, int);

int init_msq(key_t);

void consuma(int);
void produci(int, char*);




