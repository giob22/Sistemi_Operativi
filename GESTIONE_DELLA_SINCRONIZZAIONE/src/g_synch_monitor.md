# MONITOR

Prendiamo in considerazione due implementazioni del costrutto monitor: 

- `singal-and-wait` o `signal-and-wait-hoare`
- `signal-and-continue`

# PROD_CONS SINGLE BUFFER

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

