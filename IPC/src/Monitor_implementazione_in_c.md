# Monitor Implementazione in C

Per sfruttare il concetto di monitor in `c` dobbiamo realizzare una libreria sfruttando il concetto di **semaforo** per implementare tutte le funzionalità di un **monitor**.

La libreria in questione, che sia un monitor: signal-and-wait, signal-and-continue oppure signal-and-wait-Hoare; 

hanno tutte lo stesso scheletro, quello che cambia è l'implementazione di poche procedure.

L'header della libreria che implementa i monitor sarà quindi: 

`monitor.h`
```c
typedef struct {
    // id del semaforo per realizzare il mutex sul monitor, per gestire la mutua esclusione sul monitor
    int mutex;

    // nel caso in cui implementiamo il paradigma signal-and-wait-Hoaro
    // abbiamo bisogno di un semaforo per la coda urgent
    int urgent_sem

    // numero di condition variable
    int num_var_cond;

    // id del semaphor set in cui sono contenuti i semafori associati ad ogni condition variable
    int id_conds

    // id della memoria condivisa per i contatori dei processi sospesi su ogni condition variable e della coda urgent
    int id_shm;

    // array delle variabili condition count
    int *cond_counts;

    // contatore del numero di processi sospesi sulla coda urgente
    int *urgent_count;
} Monitor;
// inizializzare il monitor
void init_monitor(Monitor*,int);
// entrare nel monitor in mutua esclusione 
void enter_monitor(Monitor*);
// rilasciare il monitor per permettere ad altri processi di accedervi
void leave_monitor(Monitor*);
// eliminare il monitor, marcando come eliminabile le shm e sem in esso contenuti
void remove_monitor(Monitor*);
// procedura per sospendere un processo su una condition variable
void wait_condition(Monitor*, int);
// procedura per risvegliare un processo su una condition variable
void signal_condition(Monitor*, int);
// informa sul numero di processi sospesi su una particolare condition variable
int queue_condition(Monitor*, int);
```
