<script src="../javascript.js"></script>

# SEMAPHOR

<!--TODO aggiungi il link alla parte di teoria -->
Utilizzeremo `semid` per identificare il **file** **descriptor** dell'**array** **semaforico**

## PROD_CONS SINGOLO BUFFER

Sono necessari due semafori:

1) per gestire l'accesso dei produttori;
2) per gestire l'accesso dei consumatori.

```c
#define SPAZIO_DISP 1
#define MESS_DISP 0
```


Il semaforo per i produttori deve essere settato con valore iniziale pari al numero di buffer disponibili, in questo caso `1`.

Mentre il semaforo per i consumatori deve essere settato inizialmente a `0`, perché non possono accedere alla risorsa se non è stato prodotto ancora niente.


- PRODUTTORE:
  
  ```c
    wait_sem(semid, SPAZIO_DISP);
    // opera sul buffer singolo
    signal_sem(semid, MESS_DISP);
  ```

- CONSUMATORE
  
  ```c
  wait_sem(semid, MESS_DISP);
  // opera sul buffer singolo
  signal_sem(semid, SPAZIO_DISP);
  ```

## PROD_CONS CODA CIRCOLARE

Si utilizza una struttura come risorsa condivisa che deve tenere in conto tutte le variabili di stato per gestire correttamente una coda circolare.

```c
#define DIM_BUFF N
typedef struct {
    int buffer[DIM_BUFF];
    int testa,coda;
} Buffer;
```

Sono necessari `4` semafori per gestire questo tipo di sincronizzazione:

1) un produttore deve produrre se c'è spazio nel buffer;
2) un consumatore deve consumare se c'è un prodotto nel buffer;
3) i produttori devono accedere in modo esclusivo alle variabili di stato (`testa`);
4) i consumatori devono accedere in modo esclusivo alle variabili di stato (`coda`).

```c
#define SPAZIO_DISP 0
#define MESS_DISP 1
#define MUTEXP 2
#define MUTEXC 3
```

Il semaforo dedicato ai produttori `SPAZIO_DISP` deve essere settato con valore iniziale pari al numero di buffer liberi, ovvero `DIM_BUFF`.

Il semaforo dei consumatori `MESS_DISP` deve essere settato con valore iniziale pari a `0`, sempre perché non vogliamo che un consumatore acceda al buffer prima che non l'abbia fatto un produttore.

I due semafori per la gestione della mutua esclusione su `testa` e `coda` sono entrambi settati al valore iniziale pari ad `1` → **lock**.

- PRODUTTORE
  
  ```c
  wait_sem(semid, SPAZIO_DISP);
  wait_sem(semid, MUTEXP);
  // opero sul buffer utilizzando le variabili di stato (testa)
  signal_sem(semid, MUTEXP);
  signal_sem(semid, MESS_DISP);
  ```

- CONSUMATORE
  
  ```c
  wait_sem(semid, MESS_DISP);
  wait_sem(semid, MUTEXC);
  // opera sul buffer utilizzando le variabili di stato (coda)
  signal_sem(semid, MUTEXC);
  signal_sem(semid, SPAZIO_DISP);
  ```

## PROD_CONS POOL DI BUFFER CON VETTORE DI STATO

Si utilizza una struttura come risorsa condivisa che deve tenere conto di tutte le variabili di stato per gestire correttamente un pool di buffer e il vettore di stato associato.

```c
#define DIM_BUFF N

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct {
    int buffer[DIM_BUFF];

    int stato[DIM_BUFF]; // potrei utilizzare anche short int

} pool_buffer;
```

Sono necessari `4` semafori per gestire tale sincronizzazione:

1) un produttore può produrre solo se è presente un buffer libero;
2) un consumatore può consumare solo se è presente un buffer occupato;
3) per gestire la mutua esclusione sul vettore di stato tra produttori per la **ricerca del buffer libero**;
4) per gestire la mutua esclusione sul vettore di stato tra consumatori per la **ricerca del buffer occupato**.

```c
#define SPAZIO_DISP DIM_BUFFER
#define MESS_DISP 1
#define MUTEXP 2
#define MUTEXC 3
```

Il semaforo `SPAZIO_DISP` deve essere settato con valore iniziale pari al numero di buffer nel pool, quindi `DIM_BUFFER`.

Il semaforo `MESS_DISP` deve essere settato con valore iniziale pari a `0` perché non vogliamo che un consumatore acceda ad un valore che non è stato mai prodotto.

I semafori `MUTEXC` e `MUTEXP` devono entrambi esser settati con valore iniziale pari a `1` → sono dei **lock**.

- PRODUTTORE
  
  ```c
  wait_sem(semid, SPAZIO_DISP);
  wait_sem(semid, MUTEXP);
  // trova il primo buffer non occupato
  int i = 0;
  while (p->stato[i] != LIBERO && i < DIM_BUFFER) i++;
  p->stato[i] = IN_USO;
  signal_sem(semid, MUTEXP);
  //opera sul buffer in posizione i del pool
  p->stato[i] = OCCUPATO;
  signal_sem(semid, MESS_DISP);
  ```

- CONSUMATORE
  
  ```c
  wait_sem(semid, MESS_DISP);
  wait_sem(semid, MUTEXC);
  // trova il primo buffer occupato
  int i = 0;
  while (p->stato[i] != OCCUPATO && i < DIM_BUFF) i++;
  p->stato[i] = IN_USO;
  signal_sem(semid, MUTEXC);
  // opera sul buffer in posizione i del pool
  signal_sem(semid, SPAZIO_DISP);
  ```

## LETT_SCRITT STARVATION DEGLI SCRITTORI

Si utilizza una struttura come risorsa condivisa che deve tenere presente **il numero di lettori che stanno leggendo** oltre al valore da leggere.

```c
typedef struct {
    int valore;

    int num_lettori;
} Buffer;
```

In questo tipo di sincronizzazione sono necessari due semafori:

1) per gestire sia la cooperazione tra lettori e scrittori che la mutua esclusione tra gli scrittori stessi;
2) per gestire la mutua esclusione sulla variabile `num_lettori`.

```c
#define SYNCH 0
#define MUTEXL 1
```

Sia `SYNCH` che `MUTEXL` devono essere settato con valore iniziale pari ad `1`, nonostante possa capitare che un lettore cominci a leggere quando ancora nessuno scrittore ha inserito dati nel buffer. Sono entrambi dei **lock**.

- SCRITTORE
  
  ```c
  inizio_scrittura(int semid){
    wait_sem(semid, SYNCH);
  }
  ... // operazioni sul buffer
  fine_scrittura(int semid){
    signal_sem(semid, SYNCH);
  }
  ```

- LETTORE
  
  ```c
  inizio_lettura(int semid, Buffer* buf){
    wait_sem(semid, MUTEXL);
    buf->num_lettori++;
    // controllo se è il primo lettore 
    if (buf->num_lettori == 1) wait_sem(semid, SYNCH);
    signal_sem(semid, MUTEXL);
  }
  ... // operazioni sul buffer
  fine_lettura(int semid, Buffer* buf){
    wait_sem(semid, MUTEXL);
    buf->num_lettori--;
    // controlla se è l'ultimo lettore
    if (buf->num_lettori == 0) signal_sem(semid, SYNCH),
    signal_sem(semid, MUTXL);
  }
  ```

## LETT_SCRITT STARVATION DI ENTRAMBI

Si utilizza come risorsa condivisa una struttura in cui oltre ad essere riportato il **numero di lettori**, che stanno leggendo, anche il numero di scrittori, che sono in attesa.

```c
typedef struct {
    int valore;

    int num_lettori;
    int num_scrittori;
}
```

In questo tipo di sincronizzazione sono necessari `4` semafori:

1) per garantire la cooperazione tra scrittori e lettori sulla risorsa condivisa;
2) per gestire la mutua esclusione tra gli scrittori sulla risorsa condivisa;
3) per gestire l'accesso dei lettori alla variabile `num_lettori` in mutua esclusione;
4) per gestire l'accesso degli scrittori alla variabile `num_scrittori` in mutua esclusione.

```c
#define SYNCH 0
#define MUTEX 1
#define MUTEXL 2
#define MUTEXS 3
```

Tutti questi semafori devono essere settati con valore inziale pari ad `1`.

- SCRITTORE
  
  ```c
  inizio_scrittura(int semid, Buffer* buf){
    wait_sem(semid, MUTEXS);
    buf->num_scrittori++;
    // controlla se è il primo scrittore
    if (buf->num_scrittori == 1) wait_sem(semid, SYNCH);
    signal_sem(semid, MUTEXS)
    wait_sem(semid, MUTEX);
  }
  ... // operazioni sul buffer
  fine_scrittura(int semid, Buffer* buf){
    signal_sem(semid, MUTEX);
    wait_sem(semid, MUTEXS);
    buf->num_scrittori--;
    // controlla se è l'ultimo scrittore
    if (buf->num_scrittori == 0) signal_sem(semid, SYNCH);
    signal_sem(semid, MUTEXS);
  }
  ```

- LETTORI
  
  ```c
  inizio_lettura(int semid, Buffer* buf){
    wait_sem(semid, MUTEXL);
    buf->num_lettori++;
    // controlla se è il primo lettore
    if (buf->num_lettori == 1) wait_sem(semid, SYNCH);
    signal_sem(semdi, MUTEXL);
  }
  ... // operazione sul buffer
  fine_scrittura(int semid, Buffer* buf){
    wait_sem(semid, MUTEXL);
    buf->num_lettori--;
    // controlla se è l'ultimo lettore
    if (buf->num_lettori == 0) signal_sem(semid, SYNCH);
    signal_sem(semid, MUTEXL);
  }
  ```