// gestione della comunicazione tra processi

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "header.h"

// invece di creare 2 code, una per ogni processo, sfrutto il tipo per discriminare i messaggi dei vari processi

int main()
{

    // dove creo la coda di messaggi?

    // la risorsa viene creata dal processo starter, in modo da avere il totale controllo su questa -> sarà questo processo ad eliminarla una volta che il programma termina

    // def chiave e ottenere il descrittore

    key_t queue_key = ftok(".", 'a'); // perché P1, P2, P3 sono eseguibili separati

    // ho avuto problemi con la ftok in classe perché evidentemente il file che andavo ad 
    //indicare (che non esisteva) si trovava in una cartella in cui non avevo i permessi dell'utente.
    // ftok per funzionare utilizza dei parametri del descrittore di un file, quindi crea 
    // momentaneamente un descrittore per reperire tali informazioni e poi lo cacella
    // il problema era che non riusciva a creare il file in questione.

    int id_queue = msgget(queue_key, IPC_CREAT | 0664);

    if (id_queue > 0)
    {
        printf("descrittore della coda: %d\n", id_queue);
    }
    else
    {
        LOG("id_queue: %d", id_queue);
        exit(-1);
    }

    // devo creare 3 processi + exec per avviare p1, p2, p3
    pid_t pid = fork();
    if (pid == 0)
    {
        // creo P1
        execl("./p1", "./p1", NULL);
        printf("QUI CI ARRIVO SOLO SE HO UN ERRORE NELLA EXEC\n");
        printf("errore su P1\n");
        exit(-1);
    }
    pid = fork();
    if (pid == 0)
    {
        // creo P2
        execl("./p2", "./p2", NULL);
        printf("QUI CI ARRIVO SOLO SE HO UN ERRORE NELLA EXEC\n");
        printf("errore su P1\n");
        exit(-1);
    }
    pid = fork();
    if (pid == 0)
    {
        // creo P3
        execl("./p3", "./p3", NULL);
        printf("QUI CI ARRIVO SOLO SE HO UN ERRORE NELLA EXEC\n");
        printf("errore su P1\n");
        exit(-1);
    }

    for (int i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    msgctl(id_queue, IPC_RMID, 0);
    // qui rimuovo la coda a prescindere dal fatto che si sia qualche
    // processo in attesa di ricever un mess o qualche processo che è in attesa di
    // inviare un messaggio (se ho la coda piena e ho specificato una send bloccante)

    return 0;
}