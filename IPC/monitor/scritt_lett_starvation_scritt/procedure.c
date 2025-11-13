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
    buf->num_lettori++;
    leave_monitor(&(buf->m));
}
void fine_lettura(Buffer* buf){
    enter_monitor(&(buf->m));
    // sicuramente dovremo entrare all'interno del monitor perché c'è da modificare il numero di lettori correnti 
    // ed eventualmente segnalare gli scrittori in attesa di entrare nel monitor
    buf->num_lettori--;
    if (buf->num_lettori == 0) 
    // non ci sono più lettori, significa che il processo corrente è l'ultimo lettore
    // quindi attiva l'eventuale scrittore in attesa di accedere al monitor
        signal_condition(&(buf->m), VARCOND_SCRITTORI);
    leave_monitor(&(buf->m));
}
void inizio_scrittura(Buffer* buf){
    enter_monitor(&(buf->m));
    // verifico se sono presenti dei lettori
    if (buf->num_lettori > 0) 
    //! Nel caso utilizzassi un monitor signal-and-countinue, l'unica differenza nell'implemntazione è un
    //! while al posto di questo if → perché non si ha la sicurezza che il processo riprenda l'esecuzione subito dopo che la condizione di sincronizzazione sia valida.
        
        wait_condition(&(buf->m), VARCOND_SCRITTORI);
    // lo scrittore verifica se sono presenti lettori sulla risorsa critica; se si allora rilasciano il monitor 
    // e si sospendono sulla condition variable.
}
void fine_scrittura(Buffer* buf){
    leave_monitor(&(buf->m));
}