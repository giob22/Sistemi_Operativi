# RENDEZ-VOUS

Questo protocollo ci permette di implementare una send **sincrona** sfruttando la send asincrona e la receive bloccante fornite da System V UNIX.

Il concetto è molto simile a quello che si utilizza per il protocollo TCP.

```c
#define OTS 100
#define RTS 101

typedef struct{
    long type;
} ok_to_send;
typedef struct{
    long type;
} request_to_send;
typedef struct{
    long type;
    ... // qualsiasi dato si voglia passare
} Messaggio;

send_sincrona(Messaggio*m, int queue){
    ok_to_send ots;
    request_to_send rts;
    rts.type = RTS;
    // invio richiesta di invio
    if (msgsnd(queue, &rts, sizeof(request_to_send) - sizeof(long), 0) < 0){
        perror("Errore");
        exit(-1);
    }
    // attende l'ack
    if (msgrcv(queue, &ots, sizeof(ok_to_send) - sizeof(long), OTS, 0) < 0){
        perror("Errore");
        exit(-1);
    }
    // invia il messaggio effettivo
    if (msgsnd(queue, m, sizeof(Messaggio) - sizeof(long), 0) < 0){
        perror("Errore");
        exit(-1);
    }
}
receive_sincrona(Messaggio*m, int queue, int type_message){
    ok_to_send ots;
    request_to_send rts;
    // attende l'ack
    if (msgrcv(queue, &rts, sizeof(request_to_send) - sizeof(long), RTS, 0) < 0){
        perror("Errore");
        exit(-2);
    }
    // invia il riscontro
    ots.type = OTS;
    if (msgsnd(queue, &ots, sizeof(ok_to_send) - sizeof(long), 0) < 0){
        perror("Errore");
        exit(-2);
    }
    // attende il messaggio effettivo
    if (msgrcv(queue, m, sizeof(Messaggio) - sizeof(long), type_message, 0) < 0){
        perror("Errore");
        exit(-2);
    }
}
```