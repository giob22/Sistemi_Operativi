#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "semafori.h"

void inizio_lettura(int sem, Buffer * buf){
    // aggiorno il numero di lettori sulla shm, ma prima devo gestire la mutua esclusione su questa variabile
    wait_sem(sem, MUTEXL);
    buf->numlettori++;
    // se il processo è il primo lettore allora blocca è quello che blocca l'accesso agli scrittori
    if (buf->numlettori == 1)
    {
        wait_sem(sem, SYNCH); // blocco l'accesso agli scrittori
    }
    
    signal_sem(sem, MUTEXL);

}
void fine_lettura(int sem, Buffer* buf){
    // aggiorno la variabile numlettori
    wait_sem(sem, MUTEXL);
    buf->numlettori--;
    // se il processo è l'ultimo lettore allora permetterà agli scrittori di scrivere
    if (buf->numlettori == 0)
    {
        signal_sem(sem, SYNCH);
    }
    signal_sem(sem, MUTEXL);
    
}


int main(){
    // accedo alla shm creta dal processo master
    key_t k_shm = ftok("./AUTOVALUTAZIONE#1/main.c", 'g');

    // ottengo il descrittore verificando prima che la shm sia stata effettivamente creata

    int id_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | IPC_EXCL | 0664);
    if (id_shm < 0)
    {
        // la risorsa corrispondente a tale chiave è già presente, quindi ottengo il descruttore
        id_shm = shmget(k_shm, sizeof(Buffer), 0);
    }else
    {
        // la shm non è stata creata dal master, faccio una exit
        shmctl(id_shm, IPC_RMID, NULL);
        printf("[ANALIZZATORE: %d] la shm non è stata creata dal master\n", getpid());
        exit(1);
    }

    // attach alla shm
    Buffer* buf = (Buffer*) shmat(id_shm, NULL, 0);

    // periodicamente, ogni 2 secondi legge il contenuto della stringa, inverte il contenuto e stampa a video il risultato
    
    int indice = 0;

    while (indice < NUM_RIPETIZIONI)
    {
        indice++;
        sleep(2); 
        // sezione critica che per i lettori è la gestione della variabile numlettori
        inizio_lettura(buf->id_sem, buf);
        char t[buf->dim + 1];
        for (int i = 0; i < buf->dim; i++)
        {
            t[i] = buf->testo[buf->dim - 1 - i];
        }
        t[buf->dim] = '\0';
        printf("[ANALIZZATORE: %d] risultato dell'analisi: %s\n", getpid(), t);
        fine_lettura(buf->id_sem, buf);
    }


    return 0;
}