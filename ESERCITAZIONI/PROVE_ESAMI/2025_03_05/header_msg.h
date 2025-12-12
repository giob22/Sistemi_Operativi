#ifndef _HEADER_H
#define _HEADER_H

#include <sys/ipc.h>
#include <sys/msg.h>

/* TBD: Completare la definizione delle strutture per i messaggi.
        È possibile definire in questo file delle costanti con la direttiva "define" 
 */

#define OTS 1
#define RTS 2
#define TYPE 3

struct richiesta {
    long type;
    int valore; // valore casuale
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