#include "header.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

void * thread_lettore(void *);
void * thread_scrittore(void *);

// Variabile globale
int id_coda_sensore;

int lettura(MonitorLS * m) {

    /* 
        NOTA: C'è un unico scrittore
    */
    int my_id = syscall(SYS_gettid);

    pthread_mutex_lock(&(m->mutex));

    // condizione di sincronizzazione
    while (m->num_scrittori > 0) {
        pthread_cond_wait(&(m->cv_lett), &(m->mutex));
    }
    m->num_lettori++;
    pthread_mutex_unlock(&(m->mutex));
    int val;
    val = m->buffer;
    printf("[%d]Lettore: il valore letto = %d\n",my_id,val);
    // TODO: completare lettura
    pthread_mutex_lock(&(m->mutex));

    m->num_lettori--;
    if (m->num_lettori == 0) {
        pthread_cond_signal(&(m->cv_scritt));
    }

    pthread_mutex_unlock(&(m->mutex));
        
    return val;
}

void scrittura(MonitorLS * m, int valore) {
    
    /* 
        NOTA: C'è un unico scrittore
    */
    int my_id = syscall(SYS_gettid);
    
    pthread_mutex_lock(&(m->mutex));

    // condizione di sincronizzazione
    while (m->num_lettori > 0) {
        pthread_cond_wait(&(m->cv_scritt), &(m->mutex));
    }
    m->num_scrittori++;

    pthread_mutex_unlock(&(m->mutex));
    // TODO: completare scrittura
    m->buffer = valore;
    printf("[%d]Scrittore: il valore scritto = %d\n",my_id,m->buffer);
    
    pthread_mutex_lock(&(m->mutex));

    m->num_scrittori--;
    // non fa alcun controllo per segnalare altri scrittori perché è l'unico scrittore
    pthread_cond_broadcast(&(m->cv_lett));

    pthread_mutex_unlock(&(m->mutex));
    
}

int main() {

	srand(time(NULL));

	int key_coda = ftok(FTOK_PATH, FTOK_CHAR);
    id_coda_sensore = msgget(key_coda, IPC_CREAT | IPC_EXCL | 0664);
    if (id_coda_sensore < 0){
        // oppure id_coda_sensore == -1
        // errno = EEXIST se la coda già esiste
        if (errno == EEXIST){
            printf("\n\n\n SUCCESO \n\n");
            id_coda_sensore = msgget(key_coda, 0);
        }else{
            // qualsiasi altro errore che ha portato in fallimento l'operazione
            perror("errore");
            exit(1);
        }
    }else{
        // errore perché la message id_coda_sensore è stata appena creata
        // errore se ci si aspetta che lo sia già
        msgctl(id_coda_sensore, IPC_RMID, NULL);
        perror("errore");
        exit(1);
    }
    

    printf("Avvio processo smistatore...\n");

    pthread_t scrittore;
    pthread_t lettori[LETTORI];

    MonitorLS * m = (MonitorLS*) malloc(sizeof(MonitorLS));

    // TODO: inizializzare variabili per la mutua esclusione
    m->num_lettori = 0;
    m->num_scrittori = 0;
    

    pthread_mutex_init(&(m->mutex), NULL);
    pthread_cond_init(&(m->cv_lett), NULL);
    pthread_cond_init(&(m->cv_scritt), NULL);


    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // TODO: creare thread scrittore

    pthread_create(&scrittore, &attr, thread_scrittore, (void*)m);

    for(int i=0; i<3; i++) {
    	// TODO: creare thread lettori
        pthread_create(&lettori[i], &attr, thread_lettore, (void*)m);
    }

    // TODO: attendere la terminazione dei thread
    pthread_join(scrittore, NULL);
    for (int i = 0; i < LETTORI; i++) {
        pthread_join(lettori[i], NULL);
    }

	// TODO: deallocare la struttura
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->cv_lett));
    pthread_cond_destroy(&(m->cv_scritt));

    free(m);

    pthread_exit(NULL);

}

void * thread_lettore(void * x) {

    //TODO: recupero parametri
    MonitorLS * ls = (MonitorLS*)x; 
    srand(time(NULL) * pthread_self());

    for(int i=0; i<10; i++) {

        sleep(1);

        // TODO: leggere il valore, moltiplicarlo per un intero tra 1 e 100 e stamparlo a video
        int valore_old = lettura(ls);
        int valore = valore_old * (rand()%100 + 1);
        printf("Lettore: Stampo valore originale = %d e valore finale = %d\n", valore_old, valore);
    }

    pthread_exit(NULL);
}

void * thread_scrittore(void * x) {

    MonitorLS * ls = (MonitorLS*)x;


    for(int i=0; i<10; i++) {

        printf("Smistatore: In attesa di messaggi...\n");

        // TODO: ricezione messaggio
        Messaggio msg;
        int ret = msgrcv(id_coda_sensore, &msg, sizeof(Messaggio) - sizeof(long), TIPO, 0);
        if (ret < 0) {
            perror("[ THREAD SCRITTORE] errore nella ricezione del messaggio");
            exit(3);
        }
                
        printf("Scrittore: Ricevuto valore = %d\n", msg.value);

        // TODO: scrivere il messaggio nel monitor
        scrittura(ls, msg.value);
    }

    pthread_exit(NULL);
}
