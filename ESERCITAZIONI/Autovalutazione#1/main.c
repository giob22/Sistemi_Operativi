#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "semafori.h"





int main(){
    int st;
    // nell'implementezione di scrittori/lettori ho bisogno di due semafori, uno per la mutua esclusione tra scrittori e lettori e tra scrittori stessi,
    // l'altro per gestire la mutua esclusione sulla variabile che tine conto del numero di lettori

    // creiamo la shm richiedendo una chiave tramite ftok
    key_t k_shm = ftok("./AUTOVALUTAZIONE#1/main.c", 'g');

    // ottengo il descrittore della shm con shmget

    int id_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0664);

    // faccio l'attach

    Buffer* buf = (Buffer*) shmat(id_shm, NULL, 0);

    // inizializzo i campi della struttura

    buf->numlettori = 0;
    

    // creo l'array semaforico di 2 semafori, entrambi inizializzati ad 1

    key_t k_sem = ftok("./AUTOVALUTAZIONE#1/main.c", 'a'); // posso utilizzare IPC_PRIVATE perché dopo la creazione dell'array semaforico lo inserisco nella shm
    //key_t k_sem = IPC_PRIVATE;                             // quindi i processi figli operano direttamente sul descrittore e non hanno bisogno della chiave 
    buf->id_sem = semget(k_sem, 2, IPC_CREAT | 0664);

    // inizializzo entrambi i semafori ad 1

    semctl(buf->id_sem, SYNCH, SETVAL, 1);
    semctl(buf->id_sem, MUTEXL, SETVAL, 1);

    // creo una dimensione per la stringa di caratteri in modo casuale tra 10 e 15

    srand(time(NULL) * getpid());
    buf->dim = (rand() % 6) + 10;
    // inizializzo una stringa vuota di dim caratteri
    for (int i = 0; i < buf->dim; i++)
    {
        buf->testo[i] = 'a';
    }
    buf->testo[buf->dim] = '\0';


    // devo creare i processi figlio (generatore, elaboratore, analizzatore)
    
    // creo il generatore
    pid_t pid = fork();
    if (pid == 0)
    {
        execl("./generatore_exe", "./generatore_exe", NULL);
        printf("ERRORE se leggo questa stringa\n");
        exit(0);
    }
    pid = fork();
    if (pid == 0)
    {
        execl("./analizzatore_exe", "./analizzatore_exe", NULL);
        printf("ERRORE se leggo questa stringa\n");    
        exit(0);
    }
    for (int i = 0; i < NUM_ANALIZZATORI; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            execl("./elaboratore_exe", "./elaboratore_exe", NULL);
            printf("ERRORE se leggo questa stringa\n");    
            exit(0);
        }
    }
    
    
    

    int num_processi = NUM_ANALIZZATORI + NUM_ELABORATORI + NUM_GENERATORI;
    // il processo master attenderà la fine dei processi figli
    for (int i = 0; i < num_processi; i++)
    {
        wait(&st);
        printf("[MASTER] figlio terminato con status: %d\n", WEXITSTATUS(st));
    }


    // marco come eliminabile sia la shm che l'array semaforico
    semctl(buf->id_sem, 0, IPC_RMID);
    shmctl(id_shm, IPC_RMID, NULL);


    return 0;
}