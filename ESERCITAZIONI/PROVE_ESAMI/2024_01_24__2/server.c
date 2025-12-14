#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#include "header_msg.h"
#include "header_sem.h"

void server(int id_sem, lettscritt *p /* TBD: Completare il passaggio dei parametri */)
{

    for (int i = 0; i < 10; i++)
    {

        sleep(1);

        int valore = leggi(id_sem, p);

        printf("[SERVER WORKER %d] Letto valore %d\n", getpid(), valore);
    }
}

int main()
{

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_ots = ftok("./start.c", 'o');
    key_t key_rts = ftok("./start.c", 'r');
    key_t key_m = ftok("./start.c", 'm');

    int queue_rts = msgget(key_rts, 0);
    if (queue_rts < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: RTS queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_rts");
            exit(1);
        }
    }

    int queue_ots = msgget(key_ots, 0);
    if (queue_ots < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: OTS queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_ots");
            exit(1);
        }
    }

    int queue_msg = msgget(key_m, 0);
    if (queue_msg < 0)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "msgget: MSG queue not initialized. Run 'start' to create queues.\n");
            exit(1);
        }
        else
        {
            perror("msgget queue_msg");
            exit(1);
        }
    }

    int sem[NUM_BUFFER];
    int shm[NUM_BUFFER];
    lettscritt *ls[NUM_BUFFER];

    for (int i = 0; i < NUM_BUFFER; i++)
    {

        /* TBD: Allocare il buffer su memoria condivisa */

        shm[i] = shmget(IPC_PRIVATE, sizeof(lettscritt), IPC_CREAT | 0664);
        if (shm[i] < 0)
        {
            perror("Errore nella creazione delle shm");
            exit(3);
        }
        ls[i] = shmat(shm[i], NULL, 0);
        ls[i]->num_lettori = 0;

        printf("[SERVER FRONTEND %d] Creato buffer %d (p=%p, id=%d)\n", getpid(), i, ls[i], shm[i]);

        sem[i] = semget(IPC_PRIVATE, 2, IPC_CREAT | 0664);
        semctl(sem[i], SYNCH, SETVAL, 1);
        semctl(sem[i], MUTEXL, SETVAL, 1);
        printf("[SERVER FRONTEND %d] Creato vettore semafori %d (id=%d)\n", getpid(), i, sem[i]);
    }
    pid_t pid;

    for (int i = 0; i < NUM_SERVER; i++)
    {

        /* TBD:
            Creare un processo figlio (server worker),
            e fargli eseguire la funzione "server()".

            Passare al worker il riferimento
            a uno dei buffer e ai relativi semafori.

            Esempio: allo i-esimo processo server worker
            dovrà essere passato il semaforo "sem[i % NUM_BUFFER]".
        */

        pid = fork();
        if (pid == 0)
        {
            server(sem[i % NUM_BUFFER], ls[i % NUM_BUFFER]);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_RICHIESTE; i++)
    {

        /* TBD: Effettuare la ricezione del messaggio dal client (send sincrona) */
        ok_to_send ots;
        request_to_send rts;
        Msg msg;
        // attende rts
        int ret = msgrcv(queue_rts, &rts, sizeof(request_to_send) - sizeof(long), RTS, 0);
        if (ret < 0)
        {
            perror("Errore nella ricezione di rts");
            exit(4);
        }
        ots.type = OTS;
        ret = msgsnd(queue_ots, &ots, sizeof(ok_to_send) - sizeof(long), 0);
        if (ret < 0)
        {
            perror("Errore nell'invio di ots");
            exit(4);
        }
        ret = msgrcv(queue_msg, &msg, sizeof(Msg) - sizeof(long), TYPE_MSG, 0);

        int buffer = /* TBD */ msg.id_buff;
        int value = /* TBD */ msg.val;

        printf("[SERVER FRONTEND %d] Ricevuto richiesta (valore=%d, buffer=%d)\n", getpid(), value, buffer);

        /* TBD: Completare il passaggio dei parametri.
                Passare alla funzione i riferimenti allo
                i-esimo buffer e ai relativi semafori.
         */

        scrivi(sem[buffer], ls[buffer], value);
    }

    /* TBD: Attendere la terminazione dei processi figli e deallocare le risorse */

    for (int i = 0; i < NUM_SERVER; i++)
    {
        wait(NULL);
    }

    for (int i = 0; i < NUM_BUFFER; i++)
    {
        semctl(sem[i], 0, IPC_RMID);
        shmctl(shm[i], IPC_RMID, NULL);
    }

    return 0;
}