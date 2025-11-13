#include "procedure.h"

void inizio_lettura(Buffer*);
void fine_lettura(Buffer*);
void inizio_scrittura(Buffer*);
void fine_scrittura(Buffer*);


int lettura(Buffer* buf){
    inizio_lettura(buf);

    printf("[LETTORE: %d] valore letto: %d\n",getpid()%getppid() ,buf->buffer);
    int result = buf->buffer;

    fine_lettura(buf);

    return result;
}
void scrittura(Buffer* buf, int valore_da_scrivere){
    inizio_scrittura(buf);

    buf->buffer = valore_da_scrivere;
    printf("[SCRITTORE: %d] valore scritto: %d\n",getpid()%getppid(),buf->buffer);

    fine_scrittura(buf);
}
void inizio_lettura(Buffer* buf){
    enter_monitor(&(buf->m));
    // verifico la presenza di uno scrittore (condizione di sincronizzazione)
    if (buf->num_scrittori > 0) //! se avessi utilizzato un monitor signal-and-continue allora avrei dovuto mettere un while al posto dell'if
        wait_condition(&(buf->m), VARCOND_LETTORI);
    
    //! Se il processo scrittore non fa una signal_all per sbloccare tutti i processi lettori
    //! nel momento in cui non è presente alcun scrittore in attesa sulla loro condition variable allora:
    //! ogni lettore deve segnalare i lettori in attesa sulla condition variable per i lettori (dopo aver aggiornato il numero di lettori)
    buf->num_lettori++;
    signal_condition(&(buf->m), VARCOND_LETTORI); //! da togliere se si usa singal_all in fine scrittura
    leave_monitor(&(buf->m));
}
void fine_lettura(Buffer* buf){
    enter_monitor(&(buf->m));
    // decrementa il numero di lettori
    buf->num_lettori--;
    // se è l'ultimo lettore allora deve segnalare gli scrittori in attesa sulla loro condition variable
    if (buf->num_lettori == 0)
        signal_condition(&(buf->m), VARCOND_SCRITTORI);
    leave_monitor(&(buf->m));
}
void inizio_scrittura(Buffer* buf){
    enter_monitor(&(buf->m));
    // il processo scrittore controlla se sono presenti nel monitor scrittori o lettori
    if (buf->num_lettori > 0 || buf->num_scrittori > 0)//! se avessi utilizzato un monitor signal-and-continue allora avrei dovuto mettere un while al posto dell'if
        wait_condition(&(buf->m), VARCOND_SCRITTORI);
    // aggiorna il numero di lettori
    buf->num_scrittori++; // tale valore alla fine dei conti potrà assumere due valori possibili: 0 e 1
    leave_monitor(&(buf->m)); // lo scrittore può lasciare il monitor perché le condizioni di sincronizzazione gli permettono di operare in mutua esclusione sulla risorsa critica
}
void fine_scrittura(Buffer* buf){
    enter_monitor(&(buf->m));
    buf->num_scrittori--; // decrementa il numero di scrittori
    // uno scrittore nel momento in cui termina le sue operazioni deve dare la priorità ad altri scrittori
    // quindi controlla se sono presenti degli scrittori sulla condition variable dedicata agli scrittori
    // controlla m.cond_couts[VARCOND_SCRITTORI] e fa lo stesso per i lettori
    if (queue_condition(&(buf->m), VARCOND_SCRITTORI) > 0)
        // se sono presenti degli scrittori allora esegue una signal_cond su scrittori
        signal_condition(&(buf->m), VARCOND_SCRITTORI);
    else if (queue_condition(&(buf->m), VARCOND_LETTORI) > 0)
        signal_condition(&(buf->m), VARCOND_LETTORI); //! potrei inserire signal_all(&(buf->m), VARCOND_LETTORI) per attivarli tutti → eviterei che si debbano riattivare a vicenda successivamente.
    //? chiedi al prof se si potrebbero evitare queste verifiche dato che la signal_condition le fa già nella
    //? sua implementazione. scriverei in questo ordine:
    //? signal_condition(&(buf->m), VARCOND_SCRITTORI);
    //? signal_condition(&(buf->m), VARCOND_LETTORI);
    // sol: (mia) in realtà non sono necessari ma mi permettono di far eseguire al processo scrittore
    // sol: solo una delle due signal, in modo da garantire la mutua esclusione tra scrittori e lettori
    leave_monitor(&(buf->m));
}