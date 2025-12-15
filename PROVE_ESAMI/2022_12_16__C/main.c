#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	int key_coda = ftok(FTOK_PATH, FTOK_CHAR);
    int id_coda_sensore = msgget(key_coda, IPC_CREAT | 0664);


    if(id_coda_sensore < 0) {
        perror("Errore msgget");
        exit(1);
    }

    pid_t pid;

    pid = fork();

    if(pid == 0) {
        // TODO: processo figlio sensore (eseguibile distinto)   
        execl("./sensore", "sensore", NULL);
        perror("Errore nella exec");
        exit(1);
    }

    pid = fork();

    if(pid == 0) {
       // TODO: processo figlio smistatore (eseguibile distinto)
        execl("./smistatore", "smistatore", NULL);
        perror("Errore nella exec");
        exit(1);       	
    }


    // TODO: attendere completamento dei figli
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }
    
    // TODO: rimuovere la coda
    
        
    msgctl(id_coda_sensore, IPC_RMID, NULL);
    printf("APPLICAZIONE TERMINATA...\n");

    return 0;

}
