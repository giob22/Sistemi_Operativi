#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "semafori.h"

void inizio_scrittura(int id_sem){
    wait_sem(id_sem, SYNCH);
}

void fine_scrittura(int id_sem){
    signal_sem(id_sem, SYNCH);
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
        printf("[GENERATORE: %d] la shm non è stata creata dal master\n", getpid());
        exit(1);
    }
    
   
   // attach alla shm
   
   Buffer* buf = (Buffer*) shmat(id_shm, NULL, 0);
   
    // una volta ottenuta la shm comincia la periodica generazione di stringhe
    srand(time(NULL) * getpid());
    
    int indice = 0;
    while (indice < NUM_RIPETIZIONI)
    {
        indice++;
        sleep(3);
        //sezione Critica
        inizio_scrittura(buf->id_sem);
        sleep(1);
        // modifica un carattere della stringa
        for (int i = 0; i < buf->dim; i++)
        {
            buf->testo[i] = (char)('a' + rand() % 26);
        }
        printf("[GENERATORE: %d] la stringa generata: %s\n", getpid(),buf->testo);
        // fine sezione critica
        fine_scrittura(buf->id_sem);
    }


    return 0;
}