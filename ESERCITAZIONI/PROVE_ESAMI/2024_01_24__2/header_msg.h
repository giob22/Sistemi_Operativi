#ifndef _HEADER_MSG_
#define _HEADER_MSG_

/* TBD: Definire le strutture dati per l'invio dei messaggi */

#define OTS 23
#define RTS 22
#define TYPE_MSG 44

typedef struct{
    long type;
} ok_to_send;
typedef struct{
    long type;
} request_to_send;

typedef struct{
    long type;
    int val;
    int id_buff;
} Msg;


#define NUM_BUFFER 3
#define NUM_SERVER (NUM_BUFFER*2)
#define NUM_RICHIESTE 4

#endif