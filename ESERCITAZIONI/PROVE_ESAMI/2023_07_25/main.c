#include <complex.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procedure.h"

#define QUANTITA_INIZIALE 500
#define VALORE_INIZIALE 100

void * aggiornatore(void * x);
void * azionista(void * x);

int main() {

    MonitorPortafoglio ** m = (MonitorPortafoglio**) malloc(sizeof(MonitorPortafoglio) * 3);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    for(int i = 0; i<3; i++) {
        /* TBD: Creare ed inizializzare gli oggetti monitor */
        m[i] = (MonitorPortafoglio*) malloc(sizeof(MonitorPortafoglio));
        inizializza(m[i], QUANTITA_INIZIALE, VALORE_INIZIALE);
    }


    /* TBD: Creare il thread aggiornatore, passargli il vettore di oggetti monitor */
    pthread_t agg;

    pthread_create(&agg, &attr, aggiornatore, (void*)m);

    pthread_t azi[3];
    for(int i = 0; i<3; i++) {

        pthread_create(&azi[i], &attr, azionista, (void*)m[i]);
        /* TBD: Creare i thread azionisti, passando a ognuno uno degli oggetti monitor */
    }


    /* TBD: Attendere la terminazione del thread aggiornatore */
    pthread_join(agg, NULL);

    for(int i = 0; i<3; i++) {
        pthread_join(azi[i], NULL);
        /* TBD: Attendere la terminazione dei thread azionisti, deallocare gli oggetti */
    }


    // Devo deallocare tutto e eliminare i monitor e le cv di ogni portafoglio
    for (int i = 0; i < 3; i++) {
        termina(m[i]);
        free(m[i]);
    }
    free(m);
    printf("[MAIN] terminato con successo\n");

    return 0;
}



void * aggiornatore(void * x) {

    /* TBD: Gestire il passaggio dei parametri

        Suggerimento: Dichiarare l'array di oggetti con la sintassi "MonitorPortafoglio ** m = ..." 
    */

    MonitorPortafoglio ** m = (MonitorPortafoglio**)x;

    for(int i=0; i<10; i++) {

        for(int j=0; j<3; j++) {

            int valore = 100 + ((rand()%21) - 10);

            printf("[AGGIORNATORE] Aggiornamento titolo %d, valore %d\n", j, valore);

            
            /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor */
            aggiorna(m[j],valore);
        }

        sleep(1);
    }

    printf("[AGGIORNATORE] Terminazione\n");

    for(int j=0; j<3; j++) {

        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 200 */
        aggiorna(m[j], 200);

        sleep(1);
        aggiorna(m[j], 0);
        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 0 */
    }

    return NULL;
}


void * azionista(void *x) {

    /* TBD: Gestire il passaggio dei parametri */

    MonitorPortafoglio* m = (MonitorPortafoglio*)x; 


    printf("[AZIONISTA] Richiesta di vendita azioni, prezzo offerto 105\n");

    /* TBD: Invocare il metodo "vendita" sull'oggetto monitor, con valore 105 e quantità 50 */

    vendita(m, 50, 105);


    int valore = leggi(m)/* TBD: Invocare il metodo "leggi" */;

    printf("[AZIONISTA] Vendita effettuata, valore attuale %d\n", valore);


    printf("[AZIONISTA] Richiesta di acquisto azioni, prezzo offerto 95\n");

    /* TBD: Invocare il metodo "acquisto" sull'oggetto monitor, con valore 95 e quantità 50 */
    acquisto(m, 50, 95);

    valore = leggi(m)/* TBD: Invocare il metodo "leggi" */;

    printf("[AZIONISTA] Acquisto effettuato, valore attuale %d\n", valore);


    printf("[AZIONISTA] Terminazione\n");

    return NULL;
}