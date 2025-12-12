#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

    key_t chiave_collettore = ftok("./main.c", 'a');/*TODO*/
    int id_queue_collettore = msgget(chiave_collettore, IPC_CREAT | 0664);

    key_t chiave_server = ftok("./main.c", 'b');
    int id_queue_server = msgget(chiave_server, IPC_CREAT | 0664);

    printf("[MAIN] msq create:\ncollettore: %d\nserver: %d\n", id_queue_collettore, id_queue_server);

    //TODO: creazione ed eseuzione processo server
    pid_t pid;

    pid = fork();
    if(pid == 0){
        execl("./server", "server", NULL);
        perror("Errore nella operazione di sostituzione di testo per il server\n");
        exit(-1);
    }

    //TODO: creazione processo collettore
    pid = fork();
    if(pid == 0){
        collettore(id_queue_collettore, id_queue_server);
        exit(0);
    }


    //TODO: creazione processi sensori, ciascuno con il proprio ID
    for(int i = 0; i < NUM_SENSORI; i++){
        pid = fork();
        if(pid == 0){
            sensore(i+1,id_queue_collettore);
            exit(0);
        }
    }


    //TODO: attesa dei processi figli

    for(int i = 0; i < NUM_SENSORI + 1; i++){
        wait(NULL);
    }
    

    //TODO: rimozione risorse IPC

    msgctl(id_queue_collettore, IPC_RMID, NULL);

    return 0;
}