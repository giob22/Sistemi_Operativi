#include "header.h"

void inizializza_prod_cons(PriorityProdCons *p)
{
    init_monitor(&(p->m), 3);
    p->testa = 0;
    p->coda = 0;
    p->dim = 0;
}
void produci_alta_prio(PriorityProdCons *p)
{
    enter_monitor(&(p->m));
    // controlla la condizione di sincronizzazione
    while (p->dim == DIM_BUFFER)
    {
        wait_condition(&(p->m), PROD_HP);
    }

    // inizia la produzione ad alta priorità

    int valore_importante = rand() % 12 + 13;

    printf("[PHP] valore prodotto (%d)\n", valore_importante);

    p->buffer[p->testa] = valore_importante;
    p->testa = (p->testa + 1) % DIM_BUFFER;
    p->dim++;

    // fine produzione, riattiva i consumatori che sono in attesa

    signal_condition(&(p->m), CONS);
    leave_monitor(&(p->m));
}
void produci_bassa_prio(PriorityProdCons *p)
{
    // entrata nel monitor

    enter_monitor(&(p->m));

    // controlla la ocndizione di sincronizzazione
    while (p->dim == DIM_BUFFER || queue_condition(&(p->m), PROD_HP) > 0)
    {
        wait_condition(&(p->m), PROD_LP);
    }

    // inizia la produzione

    int valore = rand() % 12;
    printf("[PLP] valore prodotto (%d)\n", valore);

    p->buffer[p->testa] = valore;
    p->dim++;
    p->testa = (p->testa + 1) % DIM_BUFFER;

    // prima di terminare la produzione segnala ai consumatori che sono sospesi sulla loro CV
    signal_condition(&(p->m), CONS);
    leave_monitor(&(p->m));
}
void consuma(PriorityProdCons *p)
{
    // entra all'interno del monitor
    enter_monitor(&(p->m));

    // una volta entrato nel monitor controlla la condizione di sincronizzazione
    // affichè un consumatore possa consumare è necesario che il buffer non sia vuoto
    // utilizzo while perché utilizzo un monitor signal-and-continue
    while (p->dim == 0)
    {
        // allora il consumatore deve sospendersi
        wait_condition(&(p->m), CONS);
    }
    // inizia la consumazione

    int valore = p->buffer[p->coda];
    // modifico le variabili di stato del buffer
    p->coda = (p->coda + 1) % DIM_BUFFER;
    p->dim--;
    printf("[C] prodotto consumato: %d\n", valore);

    // termina la produzione uscendo dal monitor
    // e riattivando eventuali produttori nel caso siano in attesa della produzione
    // per la condizione di priorità vengono attivati sicuramente prima i produttori ad alta priorità se questi sono presenti
    if (queue_condition(&(p->m), PROD_HP))
    {
        signal_condition(&(p->m), PROD_HP);
    }
    else
    {
        signal_condition(&(p->m), PROD_LP);
    }
    // uscita dal monitor
    leave_monitor(&(p->m));
}

void rimuovi_prod_cons(PriorityProdCons *p)
{
    remove_monitor(&(p->m));
}