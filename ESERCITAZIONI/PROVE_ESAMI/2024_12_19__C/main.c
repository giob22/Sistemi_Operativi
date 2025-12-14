#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

int main() {

    /*
        TODO: creazione code di messaggi
    */
    key_t key1 = ftok("./main.c", 'a');
    key_t key2 = ftok("./main.c", 'b');

    int coda_richieste = msgget(key1, IPC_CREAT | 0664);

    int coda_risposte = msgget(key2, IPC_CREAT | 0664);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nella creazione delle code di messaggi");
        exit(1);
    }

    /*
        TODO: avvio client e server in eseguibili distinti
    */

    pid_t pid = fork();
    if (pid == 0) {
        execl("./server", "server", NULL);
        perror("Errore nella sostituzione di codice per SERVER\n");
        exit(-1);
    }
    pid = fork();
    if (pid == 0) {
        execl("./client", "client", NULL);        
        perror("Errore nella sostituzione di codice per SERVER\n");
        exit(-1);
    }

    /*
        TODO: attendo terminazione figli
    */
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }
    sleep(5);

    msgctl(coda_richieste, IPC_RMID, NULL);
    msgctl(coda_risposte, IPC_RMID, NULL);

    return 0;
}
