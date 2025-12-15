<script src="../javascript.js"></script>

# MONITOR

Prendiamo in considerazione due implementazioni del costrutto monitor: 

- `singal-and-wait` o `signal-and-wait-hoare`
- `signal-and-continue`

## PROD_CONS SINGLE BUFFER

La struttura che utiliziamo per mantenere tutte le informazioni necessarire è del tipo

```c
#define VALIDO 1
#define NON_VALIDO 0
typedef struct {
    int buffer;

    int stato;

    Monitor m;
} buffer;
```

Utilizzare un monitor per il problema consumatori produttori nel caso di un buffer singolo richiede due `condition variables`.

1) un produttore si deve sospendere se il buffer è pieno;
2) un consumatore si deve sospendere se il buffer è vuoto.

```c
#define CV_PROD
#define CV_CONS
```

- PRODUTTORE
  
  - signal-and-wait / hoare
  
    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    if (pc->stato == VALIDO) wait_condition(&(pc->m), CV_PROD);
    ...// operazioni di produzione
    pc->stato = VALIDO;
    signal_condition(&(pc->m), CV_CONS)
    leave_monitor(&(pc->m));
    ```

  - signal-and-continue
    
    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    while (pc->stato == VALIDO) wait_condition(&(pc->m), CV_PROD);
    ...// operazioni di produzione
    pc->stato = VALIDO
    signal_condition(&(pc->c), CV_CONS);
    leave_monitor(&(pc->m));
    ```

- CONSUMATORE
  - signal-and-wait / hoare
    
    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    if (pc->stato == NON_VALIDO) wait_condition(&(pc->m), CV_CONS)
    ...// operazioni di consumazione
    pc->stato = NON_VALIDO;
    signal_condition(&(pc->m), CV_PROD);
    laeve_monitor(&(pc->m));
    ```

  - signal-and-continue
    

    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    while (pc->stato == NON_VALIDO) wait_condition(&(pc->m), CV_CONS)
    ...// operazioni di consumazione
    pc->stato = NON_VALIDO;
    signal_condition(&(pc->m), CV_PROD);
    laeve_monitor(&(pc->m));
    ```


## PROD_CONS CODA CIRCLARE

Si utilizza una struttura che oltre a contenere informazioni riguardanti la risorsa condivisa, contiene anche delle variabili necessarie alla sincronizzazione:

```c
#define VARCOND_CONSUMATORI 0
#define VARCOND_PRODUTTORI 1

#define DIM N

typedef struct{
  int buffer[DIM];

  int testa, coda, dim;
  Monitor m;
} Buffer_c;
```

Il monitor deve contenere due condition variables:

1) un produttore non può produrre se la coda è piena;
2) un consumatore non può consumare se la coda è vuota.

- PRODUTTORE
  
  - signal-and-wait / hoare
  
    ```c
    enter_monitor(&(p->m));
    // condizione di sincronizzazione
    if (p->dim == DIM){
      wait_condition(&(p->m), VARCOND_PRODUTTORI);
    }
    p->dim++;
    p->buffer[p->testa] = ...// valore
    p->testa = (p->testa + 1)%DIM
    signal_condition(&(p->m), VARCOND_CONSUMATORI);
    leave_monitor(&(p->m));
    ```

  - signal-and-continue
    
    ```c
    enter_monitor(&(p->m));
    // condizione di sincronizzazione
    while (p->dim == DIM){
      wait_condition(&(p->m), VARCOND_PRODUTTORI);
    }
    p->dim++;
    p->buffer[p->testa] = ...// valore
    p->testa = (p->testa + 1)%DIM
    signal_condition(&(p->m), VARCOND_CONSUMATORI);
    leave_monitor(&(p->m));
    ```

- CONSUMATORE
  - signal-and-wait / hoare
    
    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    if (p->dim == 0){
      wait_condition(&(p->m), VARCOND_CONSUMATORI);
    }
    p->dim--;
    p->buffer[p->coda] = ...// valore
    p->coda = (p->coda + 1)%DIM;
    signal_condition(&(p->m), VARCOND_PRODUTTORI);
    leave_monitr(&(p->m));
    ```

  - signal-and-continue
    

    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    while (p->dim == 0){
      wait_condition(&(p->m), VARCOND_CONSUMATORI);
    }
    p->dim--;
    p->buffer[p->coda] = ...// valore
    p->coda = (p->coda + 1)%DIM;
    signal_condition(&(p->m), VARCOND_PRODUTTORI);
    leave_monitr(&(p->m));
    ```

## PROD_CONS VETTORE DI STATO

Si utilizza una struttura che contiene le seguenti informazioni:

```c
#define DIM N

#define PROD 0
#define CONS 1

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct{
  int buffer[DIM];
  int stato[DIM];

  int num_liberi;
  int num_occupati

  Monitor m;
}pool_buffer;
```

Il monitor dovrà essere inizializzato con due condition variales.

- PRODUTTORE
  
  - signal-and-wait / hoare
  
    ```c
    enter_monitor(&(p->m));
    // condizioni di sincronizzazione
    if (p->num_liberi == 0)
      wait_condition(&(p->m), VARCOND_PRODUTTORI);
    int i = 0;
    while(i < DIM && p->stato[i] != LIBERO) i++;
    p->stato[i] = IN_USO;
    p->num_liberi--;
    leave_monitor(&(p->m));
    ...// operazioni di produzione
    enter_monitor(&(p->m));
    p->num_occupati++:
    p->stato[i] = OCCUPATO;
    signal_condition(&(p->m), VARCOND_CONSUMATORI);
    leave_monitor(&(p->m));
    ```

  - signal-and-continue
    
    ```c
    enter_monitor(&(p->m));
    // condizioni di sincronizzazione
    while (p->num_liberi == 0)
      wait_condition(&(p->m), VARCOND_PRODUTTORI);
    int i = 0;
    while(i < DIM && p->stato[i] != LIBERO) i++;
    p->stato[i] = IN_USO;
    p->num_liberi--;
    leave_monitor(&(p->m));
    ...// operazioni di produzione
    enter_monitor(&(p->m));
    p->num_occupati++:
    p->stato[i] = OCCUPATO;
    signal_condition(&(p->m), VARCOND_CONSUMATORI);
    leave_monitor(&(p->m));
    ```

- CONSUMATORE
  - signal-and-wait / hoare
    
    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    if (p->num_occupati == 0){ wait_condition(&(p->m), VARCOND_CONSUMATORI); }
    int i = 0;
    while (i < DIM && p->stato[i] != OCCUPATO) i++;
    p->stato[i] = IN_USO;
    p->num_occupati--;
    leave_monitor(&(p->m));
    ...// operazioni di consumazione
    enter_monitor(&(p->m));
    p->num_liberi++;
    p->stato[i] = LIBERO;
    signal_condition(&(p->m), VARCOND_PRODUTTORI);
    leave_monitor(&(p->m));
    ```

  - signal-and-continue
    

    ```c
    enter_monitor(&(pc->m));
    // condizione di sincronizzazione
    while (p->num_occupati == 0){ wait_condition(&(p->m), VARCOND_CONSUMATORI); }
    int i = 0;
    while (i < DIM && p->stato[i] != OCCUPATO) i++;
    p->stato[i] = IN_USO;
    p->num_occupati--;
    leave_monitor(&(p->m));
    ...// operazioni di consumazione
    enter_monitor(&(p->m));
    p->num_liberi++;
    p->stato[i] = LIBERO;
    signal_condition(&(p->m), VARCOND_PRODUTTORI);
    leave_monitor(&(p->m));
    ```

## LETT_SCRITT STARVATION DEGLI SCRITTORI

Si utilizza una struttura dati di questo tipo:

```c
#define SYNCH 0 // gestire la cooperazione tra scrittori e lettori
#define MUTEXL 1

typedef struct {
  int buffer;

  int num_lettori;
  int num_scrittori;

  Monitor m;
} buffer;
```

Il monitor in questo caso deve essere inizializzato con due condition variables:

1) un lettore non può leggere se uno scrittore sta scrivendo;
2) uno scrittore non più scrivere se più lettori stanno leggendo.

Però è importante considerare che in questo caso il dato non è consumato, ciò significa che possono leggere contemporaneamente più processi lettori.

- SCRITTORI
  - signal-and-wait / hoare

    ```c
    inizio_scrittura(buffer* b){  
      enter_monitor(&(b->m));
      // condizione di sincronizzazione per gli scrittori
      if (b->num_lettori > 0){
        leave_monitor(&(b->m)); // esce dal monitor
        enter_monitor(&(b->m)); // si mette nuovamente in attesa del monitor
      }
      b->num_scrittori++;
    }
    ... // operazioni di scrittura
    fine_scrittura(buffer* b){
      b->num_scrittori--;
      signal_condition(&(b->m), MUTEXL);
      leave_monitor(&(b->m));
    }
    ```
  - signal-and-continue

    ```c
    inizio_scrittura(buffer* b){  
      enter_monitor(&(b->m));
      // condizione di sincronizzazione per gli scrittori
      while (b->num_lettori > 0){
        leave_monitor(&(b->m)); // esce dal monitor
        enter_monitor(&(b->m)); // si mette nuovamente in attesa del monitor
      }
      b->num_scrittori++;
    }
    ... // operazioni di scrittura
    fine_scrittura(buffer* b){
      b->num_scrittori--;
      signal_all(&(b->m), MUTEXL); // cambiamento inportante
      leave_monitor(&(b->m));
    }
    ```

- LETTORE
  - signal-and-wait / hoare

    ```c
    inizio_lettura(buffer* b){
      enter_monitor(&(b->m));
      // condizione di sincronizzazione
      if (b->num_scrittori > 0){
        wait_condition(&(b->m), MUTEXL);
      }
      b->num_lettori++;
      signal_condition(&(b->m), MUTEXL); // permette di risvegliare gli altri lettori, è da utilizzare perché lo scrittore non fa un signal_all essendo il monitor signal-and-wait
      leave_monitor(&(b->m));
    }
    ... // operazioni di lettura
    fine_lettura(buffer* b){
      enter_monitor(&(b->m));
      b->num_lettori--;
      leave_monitor(&(b->m));
    }
    ```
  - signal-and-continue

    ```c
    inizio_lettura(buffer* b){
      enter_monitor(&(b->m));
      // condizione di sincronizzazione
      while (b->num_scrittori > 0){
        wait_condition(&(b->m), MUTEXL);
      }
      b->num_lettori++;
      leave_monitor(&(b->m));
    }
    ... // operazioni di lettura
    fine_lettura(buffer* b){
      enter_monitor(&(b->m));
      b->num_lettori--;
      leave_monitor(&(b->m));
    }
    ```

## LETT_SCRITT STARVATION DI ENTRAMBI





  
