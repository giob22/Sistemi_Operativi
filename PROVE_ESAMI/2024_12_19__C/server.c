#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

typedef struct
{
    MonitorLettScritt *ls;
    int regali_richiesti;
    int pid_bambino;
} ParametriScrittore;

int coda_richieste;
int coda_risposte;

void inizializza_struttura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */
    ls->num_lettori = 0;
    ls->num_scrittori_s = 0;
    ls->num_scrittori = 0;

    pthread_mutex_init(&(ls->mutex), NULL);
    pthread_cond_init(&(ls->cv_lett), NULL);
    pthread_cond_init(&(ls->cv_scritt), NULL);
    ls->totale_regali = 0;
}

void distruggi_struttura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */

    pthread_mutex_destroy(&(ls->mutex));
    pthread_cond_destroy(&(ls->cv_lett));
    pthread_cond_destroy(&(ls->cv_scritt));
    free(ls);
}

void inizio_lettura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */
    pthread_mutex_lock(&(ls->mutex));

    // condizione di sincronizzazione
    while (ls->num_scrittori > 0)
    {
        pthread_cond_wait(&(ls->cv_lett), &(ls->mutex));
    }
    ls->num_lettori++;
    pthread_mutex_unlock(&(ls->mutex));
}

void fine_lettura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */

    pthread_mutex_lock(&(ls->mutex));

    // codizione di sincronizzazione
    ls->num_lettori--;
    if (ls->num_lettori == 0)
    {
        // ultimo lettore
        pthread_cond_signal(&(ls->cv_scritt));
    }

    pthread_mutex_unlock(&(ls->mutex));
}

void inizio_scrittura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */
    pthread_mutex_lock(&(ls->mutex));

    // condizione di sincronizzazione
    while (ls->num_lettori > 0 || ls->num_scrittori > 0)
    {
        // attende
        ls->num_scrittori_s++;
        pthread_cond_wait(&(ls->cv_scritt), &(ls->mutex));
        ls->num_scrittori_s--;
    }

    ls->num_scrittori++;

    pthread_mutex_unlock(&(ls->mutex));
}

void fine_scrittura(MonitorLettScritt *ls)
{
    /*
        TODO: completare
    */

    pthread_mutex_lock(&(ls->mutex));

    ls->num_scrittori--;
    if (ls->num_scrittori_s > 0)
    {
        // ci sono degli scrittori in attesa
        pthread_cond_signal(&(ls->cv_scritt));
    }
    else
    {
        // risveglia i lettori
        pthread_cond_broadcast(&(ls->cv_lett));
    }

    pthread_mutex_unlock(&(ls->mutex));
}

void *Scrittore(void *arg)
{
    /*
        TODO: recupero parametri thread e inizio scrittura
    */

    ParametriScrittore *ps = (ParametriScrittore *)arg;
    MonitorLettScritt *ls = ps->ls;

    inizio_scrittura(ls);

    ps->ls->totale_regali += ps->regali_richiesti;

    printf("Il totale dei regali richiesti finora è: %d\n", ps->ls->totale_regali);

    fine_scrittura(ls);

    /*
        TODO: completare
    */

    pthread_exit(NULL);
}

void *Lettore(void *arg)
{
    /*
        TODO: recupero parametri thread
    */

    ParametriScrittore *ps = (ParametriScrittore *)arg;
    MonitorLettScritt *ls = ps->ls;
    inizio_lettura(ls);

    sleep(3);

    printf("[Lettore] Il totale di regali da consegnare è: %d\n", ls->totale_regali);
    /*
        TODO: invio messaggio di risposta
    */

    MessaggioRisposta res;
    res.type = ps->pid_bambino;

    msgsnd(coda_risposte, &res, sizeof(MessaggioRisposta) - sizeof(long), 0);
    printf("Ho inviato conferma di ricezione al bambino\n");

    fine_lettura(ls);

    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    /*
        TODO: recupero code e inizializzazione strutture
    */

    key_t key1 = ftok("./main.c", 'a');
    key_t key2 = ftok("./main.c", 'b');

    coda_richieste = msgget(key1, IPC_CREAT | 0664);

    coda_risposte = msgget(key2, IPC_CREAT | 0664);

    ParametriScrittore **parametri = (ParametriScrittore **)malloc(sizeof(ParametriScrittore **) * (NUM_BAMBINI * NUM_LETTERE));
    MonitorLettScritt *ls = (MonitorLettScritt *)malloc(sizeof(MonitorLettScritt));
    inizializza_struttura(ls);

    pthread_t threads_scrittori[NUM_BAMBINI * NUM_LETTERE]; /* TODO */
    pthread_t threads_lettori[NUM_BAMBINI * NUM_LETTERE];   /* TODO */

    for (int i = 0; i < NUM_BAMBINI * NUM_LETTERE; i++)
    {
        parametri[i] = (ParametriScrittore *)malloc(sizeof(ParametriScrittore));
        parametri[i]->ls = ls;
    }
    /* TODO */

    MessaggioRichiesta msg;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < NUM_BAMBINI * NUM_LETTERE; i++)
    {
        msgrcv(coda_richieste, &msg, sizeof(MessaggioRichiesta) - sizeof(long), 0, 0);
        /*
            TODO: ricezione messaggio, inizializzazione della struttura parametri da passare al thread e creazione scrittore
        */
        parametri[i]->regali_richiesti = msg.numero_regali;
        parametri[i]->pid_bambino = msg.type;
        printf("Ho ricevuto una letterina! Sono richiesti %d regali\n", msg.numero_regali);

        pthread_create(&threads_scrittori[i], &attr, Scrittore, (void *)parametri[i]);
        pthread_create(&threads_lettori[i], &attr, Lettore, (void *)parametri[i]);
    }

    /*
        TODO: attendo terminazione thread e dealloco strutture
    */
    for (int i = 0; i < NUM_BAMBINI * NUM_LETTERE; i++)
    {
        pthread_join(threads_lettori[i], NULL);
        pthread_join(threads_scrittori[i], NULL);
    }

    distruggi_struttura(ls);
    for (int i = 0; i < NUM_BAMBINI * NUM_LETTERE; i++)
    {
        free(parametri[i]);
    }
    free(parametri);

    return 0;
}