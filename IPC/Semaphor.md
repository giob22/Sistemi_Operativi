# Semaphor

I semafori sono delle risorse IPC, ma il loro valore interno si comporta come una variabile condivisa gestita dal kernel.

Vengono utilizzati per la **cooperazione** e la **competizione** tra processi. Due o più processi possono cooperare o competere nell'esecuzione di una **sezione critica** attraverso l'uso dei semafori affinché non ci siano problemi di *sincronizzazione* o *interferenze*.

I processi condividono tra loro una **istanza di semaforo** `s`. Tramite questa variabile i processi possono sincronizzarsi sull'utilizzo di una **risorsa critica**, ovvero una risorsa condivisa.

Un processo più fare generalmente due operazioni su un semaforo `s`:
- `signal(s)`: può inviare un segnale al semaforo e in certe condizioni risvegliare i processi in attesa.
- `wait(s)`: primitiva che permette ad un processo di assicurarsi la risorsa. Nel caso la risorsa non sia disponibile perché il processo che la possiede non ha ancora inviato un segnale `signal(s)`, il processo che la richiede si sospende fino a che non riceverà il segnale `signal(s)`.

#### MODELLO CONCETTUALE SEMAFORICO:

![alt text](images/modello_concettuale_semaforico.png)

L'uso classico di un semaforo è per implementare un meccanismo di **locking** che grantisca la **mutua esclusione** tra processi.

Possiamo implementare la `wait()` e la `signal()` sfruttando quelle che sono le primitive offerte offerte dalla libreria `sys/sem.h`.

Queste due procedure ci serviranno per delimitare la sezione critica.

## Strutture semaforiche nel kernel

Il kernel Linux mantiene due strutture per implementare un semaforo: `sem` e `semid_ds`.

In particolare ogni volta che cerchiamo di istanziare un semaforo in realtà stiamo istanziando un *array* di semafori; puntatore a tale *array* è contenuto nella struttura `semid_ds`. Ogni elemento di questo array è a sua volta una struttura `sem` che descrive il singolo semaforo.

La struttura `sem` è definita in `linux/sem.h`:

```c
struct sem {
	short   sempid;         /* pid of last operation */
   	ushort  semval;         /* current value */
   	ushort  semncnt;        /* num procs awaiting increase in semval */
   	ushort  semzcnt;        /* num procs awaiting semval = 0 */
};
```

dove: 
- `sem_pid` è il PID del processo che ha eseguito l'ultima operazione su `s`;
- `sem_val` è il valore corrente del semaforo;
- `sem_semncnt` è il numero di processi che sono in attesa che una data risorsa diventi disponibile;

    (numero di processi che attendono l'incremento del semaforo)
- `sem_semzcnt` è il numero di processi che sono in attesa che il valore del semaforo sia pari a `0`

Invece la struttura che contiene l'array di semafori all'interno del sistema è `semid_ds`.

```c
/* One semid data structure for each set of semaphores in the system. */
struct semid_ds {
	struct ipc_perm sem_perm;       /* permissions .. see ipc.h */
   	time_t          sem_otime;      /* last semop time */
   	time_t          sem_ctime;      /* last change time */
   	struct sem      *sem_base;      /* ptr to first semaphore in array */
   	struct wait_queue *eventn;
   	struct wait_queue *eventz;
   	struct sem_undo  *undo;         /* undo requests on this array */
   	ushort          sem_nsems;      /* no. of semaphores in array */
};
```
dove: 
- `sem_perm` è un'istanza della struttura `ipc_perm` che è definita in `linux/ipc.h`.
  
  Questa struttura mantiene le informazioni sui permessi impostati per il semaforo.
- `sem_otime` è il tempo dell'ultima operazione `semop()`;
- `sem_ctime` è il tempo dell'ultima modifica fatta alla struttura in esame;
- `sem_base` è il puntatore al primo semaforo nell'array;
- `sem_undo` è il puntatore ad un lista di strutture, una per ogni processo che ha richiesto una *undo* durante una `semop()` sul set di semafori `sem_base`;
  
  Ovvero il numero di richieste al kernel di sistemare le situazioni in cui un processo muore prima di effettuare una `signal()` e quindi riportare il valore di `sem_val` a quello iniziale.

  Quindi il kernel, nel caso in cui un processo muore prima di poter effettuare la `signal()` e l'operazione che ha fatto utilizza il flag `SEM_UNDO`, annulla automaticamente tale operazione sul semaforo, riportandolo al valore iniziale;

  Evita situazioni di *deadlock*
- `sem_nsems` è il numero di semafori nell'array di semafori (*semaphor set*).
  
## Semaphores' workflow

Per implementare un semaforo mediante i meccanismi offerti dallo **standard systemV** è necessario l'utilizzo di alcune primitive implementate in: 
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
```

### Creazione: `semget()`

La primitiva che permette di creare un nuovo set di semafori è la system call `semget()`: 
```c
int semget(key_t key, int nsems, int semflg);
```
Tale funzione restituisce il **descrittore** del *semaphor set* associato alla chiave specificata in `key`.

Il valore della chiave è ottenibile cablando un valore, con `IPC_PRIVATE`, o con `ftok()` a seconda degli utilizzi sul semaphor set.

I flag inseribili in `semflg` sono: 
- `IPC_CREAT`;
- `IPC_EXCL`;
- Permessi di accesso in ottale.

Si possono inserire più di uno per un singolo semaphor set utilizzando l'operatore or logico `|`.

In caso di successo, la `semget()`, restituisce il descrittore del semaphor set; in caso di fallimento viene restituito `-1`.

Il numero massimo di semafori in un singolo semaphor set è definito in `linux/sem.h` come:
```c
#define SEMMSL  250
```
Questo valore dipende dalla propria architettura, per ottenere tali informazioni è necessario utilizzare il comando `ipcs -l` che restituisce diverse informazioni anche per le altre IPC.

Per creare un array semaforico di 2 semafori **con chiave** nulla (quindi i semafori saranno accessibili unicamente al processo padre e agli eventualli figli):
```c
key_t sem_key = IPC_PRIVATE;
int sem_ds = semget(key, 2, IPC_CREATE | 0644);
```

![alt text](images/creazione_semaforo.svg)

### Inizializzazione e rimozione: `semctl()`

Per poter inizializzare e rimuovere un semafor si utilizza la system call `semctl()`.

```c
int semctl(int semid, int semnum, int cmd, ...);
```

La system call esegue l'operazione specificata in `cmd` sul *semaphor set* indentificato da `semid` e sull'`semnum`-esimo semaforo dell'array.

Alcuni possibili valori da usare per `cmd` sono:
- `SETVAL`: imposta il valore, specificato come quarto parametro, di uno specifico semaforo indentificato da `semnum` all'interno del *semaphor set* `semid`;
- `IPC_RMID`: rimuove il *semaphor set* `semid` dal kernel.

In definitiva per poter un array semaforico, a due valori `val1` e `val2`:

```c
semctl(semid, 0, SETVAL, val1);
semctl(semid, 1, SETVAL, val2);
```

Per poter rimuovere un arrat semaforico (in questo caso la variabile `semnum` viene ignorata):
```c
semctl(semid, semnum, IPC_RMID);
```

### Semaphor operations: `semop()`

Per eseguire operazioni sulla struttura identificativa di un semaforo (`sem`) è necessario utilizzare la system call `semop()`:

```c
int semop(int semid, struct sembuf *sops, size_t nsops);
```

In particolare `semop()` esegue operazioni sui semafori nell'array identificato da `semid`. `sops` è un array di operazioni definite da un struttura `sembuf`, mentre `nsops` è il numero di elementi all'interno dell'array `sops`.

Ogni singola operazione è descritta da `sembuf`, una struttura contenente i seguenti campo:
```c
struct sembuf{
	unsigned short sem_num;  /* semaphore number */
   	short          sem_op;   /* semaphore operation */
   	short          sem_flg;  /* operation flags */
}
```