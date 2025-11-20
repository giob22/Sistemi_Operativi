#include <stdio.h>

#define LOG(stringa, ...) printf("[ERRORE] " stringa "\n", __VA_ARGS__)


// per problemi produttori consumatori e lettori scrittori non utilizzo mailbox
// non è il massimo utilizzare mailbox System V per tutti gli errori che si possono
// fare, in generale.

#define P1 1
#define P2 2



// per trovare il MAX_PID
// giovanni@giobPC:/proc/sys/kernel$ cat pid_max 
//4194304
// questo limite potremmo cambiarlo ricompilando il kernel

// definizione della struttura del messaggio
// prima cosa che andremo a definire

struct msg_calc
{
    // tipo messaggio
    long type; // poò essere legato al PID del processo
    // possiamo assumere questo campo per identificare il PID
    // possiamo assumere che il numero massimo di pid che possono comunicare 
    // sulla message queue è il massimo numero di PID associabili a processi → il numero massimo di processi che concorrono per la CPU
    // char nome_processo[MAX_PID]; // posso utilizzare direttamente il tipo per identificare i processi
    float numero;
};
// primo campo il tipo del messaggio che deve essere long
// può essere legato al tipo del processo
// possiamo inserire anche direttamente il pid per identificare il messaggio
// chiaramente chi riceve il messaggio deve conoscere i PID dei processi
// che possono inviare il messaggio.

// il tipo può essere utilizzato per una rcv selettiva


float generaFloat(int i_sx,int i_dx);
