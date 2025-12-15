#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"

int main()
{

    srand(getpid());

    // RICORDA DI INVERTIRE LE CODE DI MESSAGGI LATO CLIENT

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_send = ftok("./start.c", 'b');
    key_t key_rcv = ftok("./start.c", 'a');
    int send_queue = msgget(key_send, 0);
    int rcv_queue = msgget(key_rcv, 0);

    printf("[CLIENT] %d    %d\n", send_queue, rcv_queue);

    if (send_queue < 0)
    {
        perror("Errore nella creazione della coda sender lato client\n");
        exit(1);
    }

    if (rcv_queue < 0)
    {
        perror("Errore nella creazione della coda receiver lato client\n");
        exit(1);
    }

    int num_valori = 10;

    msg_init_request req;
    msg_init_response res;

    printf("[CLIENT %d] Invio richiesta (num. valori: %d)\n", getpid(), num_valori);

    /* TBD: Inviare il messaggio di richiesta */

    req.pid = getpid();
    req.valori = num_valori;

    int msgsnd_ret = msgsnd(send_queue, &req, sizeof(msg_init_request) - sizeof(long), 0);
    if (msgsnd_ret < 0)
    {
        perror("msgsnd lato client");
        printf("msgsnd returned %d\n", msgsnd_ret);
        exit(1);
    }

    /* TBD: Ricevere il messaggio di risposta */

    msgrcv(rcv_queue, &res, sizeof(msg_init_response) - sizeof(long), getpid(), 0);

    printf("[CLIENT %d] Ricevuto risposta\n", getpid());

    int id_shm_invio = res.id_shm_invio /* TBD */;
    int id_sem_invio = res.id_sem_invio /* TBD */;

    int id_shm_ricezione = res.id_shm_ricezione /* TBD */;
    int id_sem_ricezione = res.id_sem_ricezione /* TBD */;

    prodcons *p1;
    prodcons *p2;

    p1 = (prodcons *)shmat(id_shm_invio, NULL, 0);
    p2 = (prodcons *)shmat(id_shm_ricezione, NULL, 0);
    for (int i = 0; i < num_valori; i++)
    {

        int valore = rand() % 10;

        produci(id_sem_invio, p1, valore);

        printf("[CLIENT %d] Valore inviato: %d\n", getpid(), valore);

        int ricevuto = consuma(id_sem_ricezione, p2);

        printf("[CLIENT %d] Valore ricevuto: %d\n", getpid(), ricevuto);
    }
}
