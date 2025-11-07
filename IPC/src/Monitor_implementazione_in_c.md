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

## Workflow delle funzioni `wait_cond()` e `signal_cond()`

### signal-and-wait:

**`wait_cond(Monitor* m, int id_var_cond)`**:

1) Incremento il valore del contatore in `m->cond_counts[]` in posizione `id_var_cond` che identifica la posizione del contatore dei processi in attesa sulla *condition variable* `var_cond`;
2) `signal_sem(m->mutex, 0)`, rilascia il monitor in modo che altri processi possano entrarci;
3) `wait_sem(m->id_conds, id_var_cond)`, il processo si sospende sul semaforo associato alla *condition variable* identificata da `id_var_cond`;
4) nel momento in cui il processo sospeso viene riattivato decrementa il valore del contatore `m->cond_counts[]` sempre associato alla *condition variable* `id_var_cond` su cui si era sospeso in precedenza.

**`signal_cond(Monitor* m, int id_var_cond)`**:

1) Il processo verifica se ci sono processi sospesi su `id_var_cond` valutando il contatore associato a quella nell'array `m->cond_counts[]`;
2) se la condizione è verificata fa una `signal_sem(m->id_conds, id_var_cond)` in modo da riattivare un processo sospeso su tale *condition variable*;
3) altrimenti esegue una `signal_sem(m->mutex, 0)` per attivare un processo in attesa di ottenere il monitor in mutua esclusione;
4) esegue una `wait_sem(m->mutex, 0)` per sospendersi volontariamente e rientrare quindi nella coda dei processi in attesa dell'accesso al mutex.


### signal-and-wait-Hoare
   
**`wait_cond(Monitor* m, int id_var_cond)`**:

1) Incremento il valore del contatore in `m->cond_counts[]` in posizione `id_var_cond` che identifica la posizione del contatore dei processi in attesa sulla *condition variable* `var_cond`;
2) verifica la presenza di un processo in attesa sulla coda *urgent* (`*(m->urgent_counts)`) e lo riattiva con una `signal_sem(m->urgent_sem, 0)` se la condizione è verificata;
3) altrimenti esegue una `signal_sem(m->mutex, 0)` per rilasciare il monitor e permettere l'accesso ad altri processi in attesa;
4) `wait_sem(m->id_conds, id_var_cond)` si mette in attesa sul semaforo associato alla *condition variable* rappresentate la condizione di sincronizzazione che non ha soddisfatto;
5) nel momento in cui viene risvegliato decrementa il contatore in `m->cond_counts[]` in posizione `id_var_cond`.

**`signal_cond(Monitor* m, int id_var_cond)`**:

1) Incrementa il contatore dei processi in attesa sulla coda urgente, `(*(m->urgent_count))++`
2) Il processo verifica se ci sono processi sospesi su `id_var_cond` valutando il contatore associato a quella nell'array `m->cond_counts[]`;
3) se la condizione è verificata fa una `signal_sem(m->id_conds, id_var_cond)` in modo da riattivare un processo sospeso su tale *condition variable*;
4) altrimenti esegue una `signal_sem(m->mutex, 0)` per attivare un processo in attesa di ottenere il monitor in mutua esclusione;
5) esegue una `wait_sem(m->urgent_sem, 0)`, si sospende sulla coda *urgent*;
6) nel momento in cui viene riattivato decrementa il numero di processi in attesa sulla coda urgende: `(*(m->urgent_counts))--`;

### signal-and-continue