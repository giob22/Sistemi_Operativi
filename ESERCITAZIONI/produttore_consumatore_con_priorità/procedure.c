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

#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: enter_monitor (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    // controlla la condizione di sincronizzazione
    while (p->dim == DIM_BUFFER)
    {
#ifdef DEBUG
        printf("[DEBUG] produci_alta_prio: buffer full (dim=%d). waiting on PROD_HP\n", p->dim);
#endif
        wait_condition(&(p->m), PROD_HP);
#ifdef DEBUG
        printf("[DEBUG] produci_alta_prio: resumed from wait (dim=%d)\n", p->dim);
#endif
    }

    // inizia la produzione ad alta priorità

    int valore_importante = rand() % 12 + 13;

#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: produced value %d\n", valore_importante);
#endif

    printf("[PHP] valore prodotto (%d)\n", valore_importante);

    p->buffer[p->testa] = valore_importante;
#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: wrote value at index %d\n", p->testa);
#endif
    p->testa = (p->testa + 1) % DIM_BUFFER;
    p->dim++;
#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: updated state (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    // fine produzione, riattiva i consumatori che sono in attesa

#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: signalling CONS\n");
#endif
    signal_condition(&(p->m), CONS);

#ifdef DEBUG
    printf("[DEBUG] produci_alta_prio: leaving monitor\n");
#endif
    leave_monitor(&(p->m));
}
void produci_bassa_prio(PriorityProdCons *p)
{
    // entrata nel monitor

    enter_monitor(&(p->m));

#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: enter_monitor (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    // controlla la ocndizione di sincronizzazione
    while (p->dim == DIM_BUFFER || queue_condition(&(p->m), PROD_HP) > 0)
    {
#ifdef DEBUG
    if (queue_condition(&(p->m), PROD_HP))
        printf("[DEBUG] produci_bassa_prio: cannot proceed (hp_waiting=%d). waiting on PROD_LP\n", queue_condition(&(p->m), PROD_HP));
    else if (p->dim == DIM_BUFFER)
        printf("[DEBUG] produci_bassa_prio: cannot proceed (dim=%d). waiting on PROD_LP\n", p->dim);
#endif

        wait_condition(&(p->m), PROD_LP);
#ifdef DEBUG
        printf("[DEBUG] produci_bassa_prio: resumed from wait (dim=%d, hp_waiting=%d)\n", p->dim, queue_condition(&(p->m), PROD_HP));
#endif
    }

    // inizia la produzione

    int valore = rand() % 12;
#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: produced value %d\n", valore);
#endif
    printf("[PLP] valore prodotto (%d)\n", valore);

    p->buffer[p->testa] = valore;
#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: wrote value at index %d\n", p->testa);
#endif
    p->dim++;
    p->testa = (p->testa + 1) % DIM_BUFFER;
#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: updated state (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    // prima di terminare la produzione segnala ai consumatori che sono sospesi sulla loro CV
#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: signalling CONS\n");
#endif
    signal_condition(&(p->m), CONS);

#ifdef DEBUG
    printf("[DEBUG] produci_bassa_prio: leaving monitor\n");
#endif
    leave_monitor(&(p->m));
}
void consuma(PriorityProdCons *p)
{
    // entra all'interno del monitor
    enter_monitor(&(p->m));

#ifdef DEBUG
    printf("[DEBUG] consuma: enter_monitor (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    // una volta entrato nel monitor controlla la condizione di sincronizzazione
    // affichè un consumatore possa consumare è necesario che il buffer non sia vuoto
    // utilizzo while perché utilizzo un monitor signal-and-continue
    while (p->dim == 0)
    {
#ifdef DEBUG
        printf("[DEBUG] consuma: buffer empty. waiting on CONS\n");
#endif
        // allora il consumatore deve sospendersi
        wait_condition(&(p->m), CONS);
#ifdef DEBUG
        printf("[DEBUG] consuma: resumed from wait (dim=%d)\n", p->dim);
#endif
    }
    // inizia la consumazione

    int valore = p->buffer[p->coda];
#ifdef DEBUG
    printf("[DEBUG] consuma: read value %d from index %d\n", valore, p->coda);
#endif

    // modifico le variabili di stato del buffer
    p->coda = (p->coda + 1) % DIM_BUFFER;
    p->dim--;
#ifdef DEBUG
    printf("[DEBUG] consuma: updated state (dim=%d, testa=%d, coda=%d)\n", p->dim, p->testa, p->coda);
#endif

    printf("[C] prodotto consumato: %d\n", valore);

    // termina la produzione uscendo dal monitor
    // e riattivando eventuali produttori nel caso siano in attesa della produzione
    // per la condizione di priorità vengono attivati sicuramente prima i produttori ad alta priorità se questi sono presenti
    if (queue_condition(&(p->m), PROD_HP))
    {
#ifdef DEBUG
        printf("[DEBUG] consuma: signalling PROD_HP (hp waiting)\n");
#endif
        signal_condition(&(p->m), PROD_HP);
    }
    else
    {
#ifdef DEBUG
        printf("[DEBUG] consuma: signalling PROD_LP (no hp waiting)\n");
#endif
        signal_condition(&(p->m), PROD_LP);
    }

    // uscita dal monitor
#ifdef DEBUG
    printf("[DEBUG] consuma: leaving monitor\n");
#endif
    leave_monitor(&(p->m));
}

void rimuovi_prod_cons(PriorityProdCons *p)
{
    remove_monitor(&(p->m));
}