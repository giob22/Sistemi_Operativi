# Monitor Implementazione in C

Per sfruttare il concetto di monitor in `c` dobbiamo realizzare una libreria sfruttando il concetto di **semaforo** per implementare tutte le funzionalità di un **monitor**.

La libreria in questione, che sia un monitor: signal-and-wait, signal-and-continue oppure signal-and-wait-Hoare; 

hanno tutte lo stesso scheletro, quello che cambia è l'implementazione di poche procedure.

L'header della libreria che implementa i monitor sarà quindi: 

`monitor.h`

```c
typedef struct {
    // id del semaforo per realizzare il mutex sul monitor, 
    // per gestire la mutua esclusione sul monitor
    int mutex;

    // nel caso in cui implementiamo il paradigma signal-and-wait-Hoaro
    // abbiamo bisogno di un semaforo per la coda urgent
    int urgent_sem

    // numero di condition variable
    int num_var_cond;

    // id del semaphor set in cui sono contenuti i semafori 
    // associati ad ogni condition variable
    int id_conds

    // id della memoria condivisa per i contatori dei processi sospesi 
    // su ogni condition variable e della coda urgent
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
2) se la condizione è verificata fa una `signal_sem(m->id_conds, id_var_cond)` in modo da riattivare un processo sospeso su tale *condition variable*, ed esegue in seguito una `wait_sem(m->mutex, 0)` sospendendosi sul semaforo per l'accesso al mutex;
3) altrimenti se la condizione non è verificata la procedura `signal_cond(&m, id_var_cond)` è ininfluente e il processo segnalatore continua ad eseguire nel monitor. 


### signal-and-wait-Hoare
   
**`wait_cond(Monitor* m, int id_var_cond)`**:

1) Incremento il valore del contatore in `m->cond_counts[]` in posizione `id_var_cond` che identifica la posizione del contatore dei processi in attesa sulla *condition variable* `var_cond`;
2) verifica la presenza di un processo in attesa sulla coda *urgent* (`*(m->urgent_counts)`) e lo riattiva con una `signal_sem(m->urgent_sem, 0)` se la condizione è verificata;
3) altrimenti se la condizione non sia verificata esegue una `signal_sem(m->mutex, 0)` in modo da permettere ai processi in attesa del monitor di accedere;
4) il processo si sospende sulla condition variable `id_var_cond` eseguendo una `wait_sem(m->id_conds, id_var_cond)`;
5) nel momento in cui verrà riattivato il processo decrementa il contatore dei processi che sono in attesa sulla condition variable `id_var_cond`.


**`signal_cond(Monitor* m, int id_var_cond)`**:

1) Incrementa il contatore dei processi in attesa sulla coda urgente, `(*(m->urgent_count))++`
2) Il processo verifica se ci sono processi sospesi su `id_var_cond`, valutando il contatore associato a questa nell'array `m->cond_counts[]`;
3) se la condizione è verificata fa una `signal_sem(m->id_conds, id_var_cond)` in modo da riattivare un processo sospeso su tale *condition variable* e successivamente esegue `wait_sem(m->urgent_sem, 0)` sospendendosi sulla coda urgente;
4) altrimenti la procedura `signal_cond(&m, id_var_cond)` è ininfluente, il contatore dei processi in attesa nella coda urgente viene decrementato `(*(m->urgent_count))--` e il processo continua ad eseguire all'interno del monitor.

### signal-and-continue

**`wait_condition(Monitor* m, int id_var_cond)`**

1) il processo incrementa il contatore di processi in attesa sulla *condition variable* eseguendo `m->cond_counts[id_var_cond]++`;
2) il processo esegue una `signal_sem(m->mutex, 0)` in modo da permettere ad altri processi di accedere al monitor;
3) successivamente si sospende volontariamente sulla *condition variable*, il cui identificatore è passata per parametro, mediante una `signal_sem(m->var_conds, id_var_cond)`;
4) nel momento in cui viene riattivato da un processo segnalatore che effettua una `signal_sem()` sulla stessa *condition variable* riprende l'esecuzione facendo una `wait_sem(m->mutex, 0)` per sospendersi sulla coda dei processi in attesa del monitor.
   
   Questo perché  ci troviamo in un monitor **signal-and-continue** quindi nel momento in cui un processo sospeso una una variabile condition viene sbloccato questo deve ritornare nella coda dei processi che attendono di ottenere il monitor. Altrimenti ci saranno due processi che operano all'interno del monitor.

**`signal_cond(Monitor* m, int id_var_cond)`**

1) verifica se sono presenti dei processi in attesa sulla *condition variable* verificando il contatore in `m->cond_counts[id_var_cond]`;
2) se sono presenti dei processi in attesa allora esegue una `signal_sem(m->var_conds, id_var_cond)` e decrementa il numero di processi in attesa sulla variabile condition `m->cond_counts[id_var_cond]--`;
3) altrimenti la chiamata è ininfluente.

In entrambi i casi, sia che la verifica sia vero oppure falsa, il processo segnalatore continua la sua esecuzione all'interno del monitor.

<!--todo aggiungi il codice sorgente per ogni workflow   -->