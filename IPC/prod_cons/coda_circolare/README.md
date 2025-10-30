
---

# 🧵 Produttore–Consumatore (IPC System V in C)

## 📖 Descrizione

Implementazione del problema **produttore–consumatore** usando **memoria condivisa (Shared Memory)** e **semafori System V** per sincronizzare **più produttori** e **più consumatori** che accedono a un **buffer circolare**.

## 📂 Struttura

```
.
├── Makefile
├── main.c
├── procedure.c
├── procedure.h
├── semafori.c
└── semafori.h
```

### `procedure.h`

Costanti e tipi principali:

```c
#define SPAZIO_DISP 0
#define MESS_DISP   1
#define MUTEX_C     2
#define MUTEX_P     3

#define NUM_PRODUTTORI  5
#define NUM_CONSUMATORI 5
#define DIM_BUFFER      5

struct pool_buffer {
    int buffer[DIM_BUFFER];
    int testa, coda;
};
```

Prototipi:

```c
void produttore(struct pool_buffer*, int /* ds_sem */);
void consumatore(struct pool_buffer*, int /* ds_sem */);
```

### `semafori.[ch]`

Wrapper su `semop()`:

```c
int wait_sem(int id_sem, int num_sem);   // P()
int signal_sem(int id_sem, int num_sem); // V()
```

### `procedure.c`

Logica di **produttori** e **consumatori** (notare l’uso a **due argomenti** dei wrapper semaforici):

Produttore:

```c
wait_sem(ds_sem, SPAZIO_DISP);
wait_sem(ds_sem, MUTEX_P);
// sezione critica: produce e aggiorna testa
signal_sem(ds_sem, MUTEX_P);
signal_sem(ds_sem, MESS_DISP);
```

Consumatore:

```c
wait_sem(ds_sem, MESS_DISP);
wait_sem(ds_sem, MUTEX_C);
// sezione critica: consuma e aggiorna coda
signal_sem(ds_sem, MUTEX_C);
signal_sem(ds_sem, SPAZIO_DISP);
```

### `main.c`

* Crea **shared memory** per `struct pool_buffer` e inizializza `testa`, `coda` e `buffer`.
* Crea l’**array di 4 semafori** e li inizializza:

  * `SPAZIO_DISP = DIM_BUFFER`
  * `MESS_DISP   = 0`
  * `MUTEX_P     = 1`
  * `MUTEX_C     = 1`
* Fa `fork()` di `NUM_PRODUTTORI` e `NUM_CONSUMATORI`.
* Attende i figli (`wait()`), **detacha** la SHM (`shmdt(p)`) e marca SHM e semafori per la rimozione (`IPC_RMID`).

## ▶️ Compilazione ed esecuzione

```bash
make
./main
# pulizia
make clean
```

## 🧪 Output verosimile (multi-produttore/consumatore)

> L’ordine e i PID cambiano a ogni esecuzione.

```
[PID: 4123] CONSUMATORE avviato
[PID: 4124] CONSUMATORE avviato
[PID: 4125] CONSUMATORE avviato
[PID: 4126] CONSUMATORE avviato
[PID: 4127] CONSUMATORE avviato
[PID: 4132] PRODUTTORE avviato
[PID: 4133] PRODUTTORE avviato
[PID: 4134] PRODUTTORE avviato
[PID: 4135] PRODUTTORE avviato
[PID: 4136] PRODUTTORE avviato
[PID: 4132] PRODUTTORE produce:     684
[PID: 4123] CONSUMATORE consuma:    684
[PID: 4134] PRODUTTORE produce:     101
[PID: 4136] PRODUTTORE produce:     927
[PID: 4126] CONSUMATORE consuma:    101
[PID: 4133] PRODUTTORE produce:     245
[PID: 4125] CONSUMATORE consuma:    927
[PID: 4135] PRODUTTORE produce:     772
[PID: 4127] CONSUMATORE consuma:    245
[PID: 4124] CONSUMATORE consuma:    772
processo terminato ...
```

##  Concetti chiave

* **Cooperazione** tra processi tramite `SPAZIO_DISP` e `MESS_DISP`.
* **Mutua esclusione** con `MUTEX_P` e `MUTEX_C` per evitare race condition su `testa`/`coda`.
* **Pulizia risorse** con `shmdt`, `shmctl(... IPC_RMID ...)`, `semctl(... IPC_RMID ...)`.

---
