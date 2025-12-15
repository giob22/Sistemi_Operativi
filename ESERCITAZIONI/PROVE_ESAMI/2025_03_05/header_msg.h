#ifndef _HEADER_H
#define _HEADER_H

#include <sys/ipc.h>
#include <sys/msg.h>

/* TBD: Completare la definizione delle strutture per i messaggi.
        È possibile definire in questo file delle costanti con la direttiva "define" 
 */
#define OK_TO_SEND 90
#define REQUEST_TO_SEND 99
#define MESSAGE 66

struct richiesta {
    long type;
    int value;
    /* TBD */
};

struct rts {
    /* TBD */
    long type;
};

struct ots {
    /* TBD */
    long type;
};

#endif