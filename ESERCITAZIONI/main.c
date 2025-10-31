#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "coda_richieste.h"

#define DIM_DISCO 20 

/* 
    Lo schedulatore invoca preleva_richiesta
*/
void Schedulatore(coda_richieste *c)
{

    int disco[DIM_DISCO];
    int posizione_corrente = 0;

    richiesta r;

    for (int i = 0; i < DIM_DISCO; i++)
    {

        preleva_richiesta(c, &r);

        printf("[%d] Prelevo richiesta: posizione=%d, processo=%d\n", getpid(), r.posizione, r.processo);

        int attesa = abs(posizione_corrente - (int)r.posizione);

        printf("[%d] Attesa Schedulatore... (%d secondi)\n", getpid(), attesa);

        sleep(attesa);

        disco[r.posizione] = r.processo;

        posizione_corrente = r.posizione;
    }
}

void Utente(coda_richieste *c)
{

    srand(getpid());

    richiesta r;

    for (int i = 0; i < 10; i++)
    {

        r.processo = getpid();
        r.posizione = rand() % DIM_DISCO;

        printf("[%d] Richiesta Utente: posizione=%d, processo=%d\n",getpid(), r.posizione, r.processo);

        inserisci_richiesta(c, &r);
    }
}

int main()
{

    coda_richieste * c;

    // inizializza_coda ritorna un puntato alla shm che include
    /*
        c->testa = 0;
        c->coda = 0;
        c->sem_id = sem_id; 
        c->shm_id = shm_id; 
    */
    c = inizializza_coda();

    // TODO: creazione di 1 processo che invochi la funzione Schedulatore(...)
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("erorre nella creazione dello schedulatore\n");    
    }
    if (pid == 0)// contesto schedulatore
    {
        printf("[SCHEDULATORE: %d] avviato\n", getpid());
        Schedulatore(c);
        exit(1);
    }
    
    
    
    for (int i = 0; i < 2; i++)
    {

        // TODO: creazione di 2 processi che invocano la funzione Utente(...)
        pid = fork();
        if (pid < 0)
        {
            printf("erorre nella creazione dello dell'utente %d\n", i +1);    
        }
        if (pid == 0) // contesto utente
        {
            printf("[UTENTE: %d] avviato\n", getpid());
            Utente(c);
            exit(2);
        }
        
        
    }


    for(int i=0; i<3; i++) {
        // TODO: attendere la terminazione dei processi
        wait(NULL);
    }


    rimuovi_coda(c);
    printf("PROGRAMMA TERMINATO...\n");

}
