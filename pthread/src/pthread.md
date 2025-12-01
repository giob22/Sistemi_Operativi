# pthread

**thread** possono essere definiti come un flusso di esecuzione all'interno di un processo (*leggero*).

Possiamo utilizzarli importando la libreria `pthread.h`.

Sarebbe una libreria user level che implemente le API POSIX per la gestione dei thread.

Essere **POSIX compliant** (conforme a POSIX) significa che la libreria fornisce un insieme di funzioni standard che seguono le specifiche POSIX (Portable Operating System Interface), un insieme di norme che definiscono come i sistemi operativi devono gestire vari aspetti del software.

(**importante per la portabilità**)

Questa è una grande differenza rispetto i meccanismi che abbiamo visto fin'ora che sono **System V compliant**:

- `shm`;
- `sem`;
- `msq`.

Non tutti i sistemi moderni, infatti, supportano System V al contrario di POSIX.

Per poter utilizzare questa libreria **dinamica**, dobbiamo comunicarlo al compilatore mediante un flag `-pthread` o `-lpthread` nel momento in cui compiliamo il codice.

<!-- @todo scrivi il motivo per cui dobbiamo inserire questo flag -->

Infatti i PThread sono definiti da una libreria in C composta dal file di intestazione `pthread.h` e una libreria dinamica. Il flag di compilazione `-pthread` deve essere utilizzato per compilare correttamente il codice che utilizza PThread.

### Motivi per cui si utilizzano pthread

Il motivo principale è dovuto all'**overhead**, infatti si ha un guadagno di prestazioni nell'utilizzare gli pthread al posto di nuovi processi.

I thread sono più leggeri rispetto ai processi, principalmente per due motivi:

- **condivisione dello spazio di indirizzamento**: i thread all'interno dello stesso processo condividono lo stesso spazio di indirizzamento della memoria, mentre i processi hanno spazi di indirizzamento separati.
  
  Questo significa che la creazione di un nuovo thread non richiede l'allocazione di un intero spazio di indirizzamento, come avviene per i processi;
- **Minor overhead per la gestione**: i thread non necessitano della creazione di un **Process Control Block** completo come per i processi. Ogni thread ha una struttura dati associata che contiene solo informazioni relative al suo stato.

Quando viene creato un nuovo thread, il sistema operativo deve solo allocare risorse per il thread stesso (come lo stack e il contesto di esecuzione), ma **non crea un nuovo spazio di indirizzamento separato** come nel caso di un processo. Inoltre, i thread possono comunicare facilmente tra loro perché operano nello stesso spazio di indirizzamento se fanno parte dello stesso processo (ogni thread può vedere l'intero spazio di indirizzamento).

Proprio perché si trovano nello stesso contesto non dobbiamo creare aree di memoria dedicate

- (**gestite dal kernel**, mediante le system call → cambio di contesto, overhead)

per la comunicazione tra thread, possiamo sfruttare lo stesso spazio di indirizzamento: in particolare lo heap.

<p align='center'><img src='images/vantaggi_pthread.png' width='500' ></p>

- Una `fork()`, in particolare un `clone()`, impiega molto più tempo per creare un processo rispetto a `pthread_create()`

ESEMPIO DI PRODOTTO SCALARE:

- si vuole calcolare il prodotto scalare di due vettori di numeri reali
- i vettori possono essere condivisi tra più thread
- ogni thread calcola il prodotto scalare tra una parte dei due vettori, ed aggiorna una variabile condivisa contenente il risultato.

<p align='center'><img src='images/esempio_prdotto_scalare.png' width='500' ></p>

- possiamo calcolare il tempo che impiega un'applicazione multiprocess e un'applicazione multithread.

## API di programmazione

- **Gestione dei thread**: creazione, distribuzione e join di thread
- **Gestione dei Mutex**: creazione, distribuzione, lock e unlock di variabili di mutua esclusione (mutex) per la gestione di sezioni critiche.
- **Gestione delle Condition Variables**: creazione, distribuzione, wait e signal su condition variables definite dal programmatore.

### Creazione di un thread

```c
int pthread_create(pthread_t *restrict thread,
                   const pthread_attr_t *restrict attr,
                   void *(*start_routine)(void *),
                   void *restrict arg);
```

Questa funzione crea un nuovo thread nel processo chiamante. Tale thread inizia ad eseguire dall'*entry point* passatogli per parametro `start_routine()`; dove `arg` è l'argomento passato per parametro alla funzione `start_routine()`.

NOTA: il nome di un funzione, come per gli array, definisce l'entry point/puntatore alla prima istruzione della funzione.

L'entry point della funzione che passiamo per parametro deve avere un **prototipo specifico**:

```c
void* start_routine(void*);
```

Importante notare che è possibile passare solo un argomento per parametro alla funzione `start_routine()`, quindi nel caso dovessimo passare più di un argomento è necessario creare una `struct` che li contenga tutti.

`pthread_t *thread` è un puntatore che riceverà l'identificatore del nuovo thread creato. `pthread_t` è un tipo di dato che rappresenta l'ID del thread e nella funzione è un parametro di input/output.

`const pthread_attr_t *attr` è un puntatore alla struttura di tipo `pthread_attr_t` che definisce gli **attributi del thread**, come la sua priorità, politica di schedulazione, la dimensione dello stack, ecc.

- Nel caso si voglia utilizzare gli attributi predefiniti di un thread si inserisce come parametro `NULL`

> `restrict` in C è un suggerimento al compilatore che un determinato puntatore è l'unico modo per accedere alla memoria puntata a cui esso punta, all'interno del blocco di codice in cui è dichiarato.\
> → Permette di ottimizzare meglio l'accesso alla memoria in quanto garantisce che i puntatori non vengano utilizzati per fare riferimento allo stesso oggetto in modo concorrente.

### Terminazione di un pthread

Un thread può **terminare** per diversi motivi:

- termina l'esecuzione della `start_routine()` uscendo fuori dallo scope di tale funzione;
-  il thread chiama la funzione `pthread_exit()`;
-  il thread è concellato da un altro thread con `pthread_cancel()`;
-  L'intero processo termina.

Nel caso si ammette l'utilizzo di `pthread_cancel()` perdiamo il potere del **thread master** sulla vita dei thread creati.


<!-- 15:55 -->
