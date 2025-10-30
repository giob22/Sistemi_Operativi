

---

# 🧵 Produttore–Consumatore ()

## 📖 Descrizione

Questo progetto contiene un **secondo programma** che utilizza le stesse interfacce di semafori e procedure del primo progetto. I file mostrano l’uso delle primitive **IPC System V** (semafori e memoria condivisa) e la presenza di processi figli gestiti con `wait()`.


---

## 📂 Struttura della cartella

```
.
├── Makefile
├── main.c
├── procedure.c
├── procedure.h
├── semafori.c
└── semafori.h
```

---

## 📌 Contenuto dei file

### `main.c`

File **main** alternativo che include:

* Header di progetto: `semafori.h`, `procedure.h`
* Header di sistema per IPC e processi:

  * `sys/ipc.h`, `sys/sem.h`, `sys/shm.h`
  * `sys/wait.h`, `sys/types.h`
  * `stdlib.h`, `unistd.h`, `time.h`, `stdio.h` (tramite gli header standard)
* Commenti che indicano le tre macro-aree: gestione semafori, gestione SHM, lifecycle dei processi.

Dal codice **presente** (dopo il tratto troncato `...`) si vedono:

* **Due** chiamate a `wait(NULL);` per attendere **due processi figli**.
* Stampa conclusiva: `printf("processo terminato ...\n");`
* **Rilascio risorse IPC**:

  * `semctl(ds_sem, 0, IPC_RMID);`  → marcare **il set di semafori** per la rimozione.
  * `shmctl(ds_shm, IPC_RMID, NULL);` → marcare la **SHM** per la rimozione.
* `return 0;`



---

### `procedure.h`

* **Indici dei semafori** e costanti:

  ```c
  #define SPAZIO_DISP 0
  #define MESS_DISP   1
  #define MUTEX_C     2
  #define MUTEX_P     3

  #define NUM_PRODUTTORI  5
  #define NUM_CONSUMATORI 5
  #define DIM_BUFFER      5
  ```
* **Struttura condivisa**:

  ```c
  struct pool_buffer {
      int buffer[DIM_BUFFER];
      int testa, coda;
  };
  ```
* **Prototipi**:

  ```c
  void produttore(struct pool_buffer*, int);
  void consumatore(struct pool_buffer*, int);
  ```

### `procedure.c`

* Implementa `produttore()` e `consumatore()` usando le funzioni di sincronizzazione dichiarate in `semafori.h`.
* Le funzioni fanno uso degli indici/semantica definiti in `procedure.h` (buffer circolare con `testa` e `coda`).

### `semafori.h` e `semafori.c`

* **Wrapper** per le operazioni sui semafori System V:

  ```c
  int wait_sem(int id_sem, int num_sem);
  int signal_sem(int id_sem, int num_sem);
  ```
* Internamente costruiscono una `struct sembuf` e invocano `semop()` per effettuare le classiche operazioni P (−1) e V (+1) su un semaforo del set.

### `Makefile`

* Target principale: `main_prodcons_CC`
* Compilazione dei sorgenti:

  ```
  all: main_prodcons_CC

  main_prodcons_CC: main.o semafori.o procedure.o
  	gcc -o main_prodcons_CC main.c semafori.o procedure.o

  main.o: main.c semafori.h procedure.h
  	gcc -c main.c

  semafori.o: semafori.c semafori.h
  	gcc -c semafori.c

  procedure.o: procedure.c procedure.h
  	gcc -c procedure.c

  clean:
  	rm -r *.o
  	rm -r main_prodcons_CC
  ```


---

## ▶️ Compilazione ed esecuzione

### Opzione A – usare il Makefile esistente (compila l’altro programma)

```bash
make
./main_prodcons_CC
```

### Opzione B – compilare **questo** programma (il main alternativo) direttamente da riga di comando

Usando i file presenti in cartella:

```bash
gcc -std=c99 -Wall -Wextra -O2 \
  main.c \
  semafori.c procedure.c \
  -o main_alt
./main_alt
```

---

## 🧠 Cosa dimostrano i file presenti

* Organizzazione di un progetto **IPC System V** con separazione tra:

  * interfacce di sincronizzazione (`semafori.[ch]`),
  * logica applicativa produttore/consumatore (`procedure.[ch]`),
  * programma principale (`main.c`).
* Presenza di **processi figli** (due `wait(NULL)` indicano l’attesa di **due** figli).
* **Rilascio** delle risorse di IPC con `IPC_RMID` per **semafori** e **shared memory**.

---

