
#include "monitor_fifo.h"

#include <stdio.h>
#include <unistd.h>

void inizializza(MonitorProdConsFIFO *p)
{
    
// inizializzo il monitor
init_monitor(&(p->m), 2);
// due condition variables
// un produttore non può produrre se il buffer è pieno
// un consumatore non può consumare se il buffer è vuoto

// stato vuoto per tutti gli buffer
for (int i = 0; i < DIM; i++)
{
    p->stato[i] = VUOTO;
}

p->num_occupati = 0;
p->num_liberi = DIM;

p->ultimo_produttore_arrivato = 0;
p->ultimo_produttore_avviato = 0;
}

void produci(MonitorProdConsFIFO *p, int valore)
{
    
/* TBD: Completare con la sincronizzazione */

// entrata nel monitor
enter_monitor(&(p->m));

int arrivo = p->ultimo_produttore_arrivato;

printf("%d\n", p->ultimo_produttore_arrivato);

p->ultimo_produttore_arrivato++;

printf("[PRODUTTORE %d] Il mio turno di arrivo è %d\n", getpid(), arrivo);

// ... gestire le condizioni di attesa ...

// costrutto monitor signal_and_continue, quindi utilizzo while perché non è detto che alla riattivazione la condizione di sincronizzazione sia ancora valida
while (p->num_liberi == 0 || arrivo != p->ultimo_produttore_avviato)
{
    wait_condition(&(p->m), PROD);
}

// ottengo il primo buffer libero per la produzione
int i = 0;
while (p->stato[i] != VUOTO)
{
    i++;
}

// ottengo l'indice del buffer VUOTO
// lo setto come IN USO
p->stato[i] = USO;
// decremento il numero di buffer liberi
p->num_liberi--;

printf("[PRODUTTORE %d] Il mio turno di produzione è %d\n", getpid(), p->ultimo_produttore_avviato);

p->ultimo_produttore_avviato++;

// il produttore esce dal monitor

leave_monitor(&(p->m));

// inizia la produzione...

p->buffer[i] = valore;

// sleep(1);

// rientra nel monitor per modificare le variabili di stato e risvegliare eventuali consumatori in attesa
enter_monitor(&(p->m));

// setto lo stato del buffer come PIENO
p->stato[i] = PIENO;

// incremento il numero di buffer pieni
p->num_occupati++;

// segnala i consumatori

signal_condition(&(p->m), CONS);

// uscita dal monitor
leave_monitor(&(p->m));
}

int consuma(MonitorProdConsFIFO *p)
{
    
    /* TBD: Completare con la sincronizzazione */
    // entrata nel monitor
    enter_monitor(&(p->m));
    
    // verifica della condizione di sincronizzazione
    // devo utilizzare necessariamente occupati, perché se utilizzassi num_liberi != DIM nel caso entrino
    // due consumatori e DIM = DIM - 1, allora il primo entra senza problemi e trova il valore i, il secondo entra comunque nonostante non ci siano più spazi
    // questo perché il consumatore non incrementa subito il numero di spazi liberi, ma decrementa quello degli spazi occupati
    while (p->num_occupati == 0)
    {
        wait_condition(&(p->m), CONS);
    }
    
    // ricerca del primo buffer vuoto
    int i = 0;
    while (p->stato[i] != PIENO)
    {
        i++;
    }
    
    // decremento del numero di buffer pieni
    p->num_occupati--;
    // set dello stato del buffer in USO
    p->stato[i] = USO;
    
    printf("[CONSUMATORE %d] Consumazione in corso\n", getpid());
    
    // esce dal monitor per permettere ad altri consumatori o produttori di entrarci
    leave_monitor(&(p->m));
    
    // inizio consumazione
    
    int valore = p->buffer[i];
    
    // sleep(1);
    
    // rientra nel monitor
    enter_monitor(&(p->m));
    
    // modifica delle variabili di stato
    p->num_liberi++;
    p->stato[i] = VUOTO;
    
    /* Nota: Occorre riattivare -tutti- i produttori in attesa.
    Il numero di produttori in attesa è dato dalla seguente differenza.
    */
   
   // potrei fare anche una signal_all? SI però il professore vuole che faccia in questo modo
   
   int produttori_in_attesa = p->ultimo_produttore_arrivato - p->ultimo_produttore_avviato;
   
   signal_all(&(p->m), PROD);
   // uscita dal monitor
   leave_monitor(&(p->m));
   
   return valore;
}

void distruggi(MonitorProdConsFIFO *p)
{
    
    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&(p->m));
}
