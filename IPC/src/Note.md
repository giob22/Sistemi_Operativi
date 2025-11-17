# NOTE

## Utilizzo di semafori tra processi con fork + exec

**PROBLEMA DELLA VALIDITÁ DEL DESCRITTORE DELL'ARRAY SEMAFORICO:**

Supponiamo di avere un processo che crea altri `N` processi con `fork() + exec()` oppure in generale più processi con area testo diversa che operano su una `shm` il cui accesso è gestito dall'utilizzo di semafori.

Possiamo avere diverse implementazioni che apparentemente potrebbero sembrare valide, ma in realtà nascondono un problema legato alla validità del descrittore dell'array semaforico.

(vedi prova di autovalutazione#1)

due potrebbero essere le soluzioni:

1) il padre fa una `shmget + shmat` e `semget` (utilizzando una chiave generata con `ftok`) creando le risorse IPC e in tutte le applicazioni generate con `fork + exec` si ripete `shmget + shmat` e `semget;
2) il padre fa una `shmget + shmat` e `semget` creando le risorse IPC, ma mettendo nella struttura dati che descrive la risorsa condivisa da proteggere anche l'identificativo dell'array semaforico.

   ```c
    struct SharedData{
        int semid;
        // ...
    }
   ```

   Quindi nelle applicazioni create con `fork + exec` si eseguirà solo `shmget + shmat` (utilizzando la stessa chiave generata con `ftok`) ottenendo il descrittore dell'array semaforico accedendo alla struttura dati condivisa.

La seconda soluzione, seppur valida e funzionante pone un rischio dal punto di vista della **validità del descrittore** se qualche processo prova a cancellare l'array semaforico. Perché essendo che la rimozione fisica dell'array semaforico legata alla `semop()`, questo potrebbe essere eliminato definitivamente se nessun processo è in attesa su un semaforo.

Questo quindi vuol dire che nel momento in cui un altro processo che ha ottenuto direttamente il costruttore (tramite la `shm`) prova a utilizzare il semaforo potrebbe ottenere un comportamento inaspettato.

Ovviamente questo problema può presentarsi anche nel caso in cui il processo figlio, creato con `fork + exec`, tenti di utilizzare un semaforo che inizialmente esiste e di cui ottiene il descrittore tramite `semget()`, usando la stessa chiave del processo padre. Tuttavia, prima che il semaforo non sia più necessario all’esecuzione del figlio, il padre potrebbe marcarlo come **eliminabile**.

In questa situazione, nonostante il semaforo possa essere necessario al processo figlio che possiede un descrittore valido, esso potrebbe comunque essere eliminato fisicamente, poiché l’eliminazione di un array di semafori dipende da `semop()`. In particolare, un semaforo marcato come eliminabile non viene rimosso fisicamente finché esiste un processo in attesa su almeno uno dei semafori dell’array.

Nel mio **git hub** ho simulato questa situazione con due processi che eseguono due codici diversi ma utilizzano la stessa chiave per ottenere il descrittore del semaforo su cui entrambi lavorano.

Sono contenuti all'interno della cartella `IPC/eliminazione_semafori`. Ecco l'output su terminale:

```console
[PROGRAMMA 1] chiave: 1644457381
[PROGRAMMA 1] il descrittore del semaforo é: 9
[PROGRAMMA 1] eseguo una ipcs per verificare a terminale se è presente il semaforo
[PROGRAMMA 2] chiave: 1644457381
valore di ds_sem: -1
il semaforo é stato creato correttamente dal programma1.c
ho ottenuto il semaforo esistente
[PROGRAMMA 2] il descrittore del semaforo é: 9
[PROGRAMMA 2] faccio una signal sul semaforo per sbloccare il programma1 (adesso la coda di attesa del semaforo è vuota!!)
[PROGRAMMA 2] sto per eseguire un'operazione pesante che non mi permetterà di accedere immediatamente sul semaforo

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x62046da5 9          giovanni   644        1         

[PROGRAMMA 1] mi metto in attesa sul semaforo
[PROGRAMMA 1] sono stato sbloccato
[PROGRAMMA 1] marco come eliminabile il semaforo
[PROGRAMMA 2] eseguo una ipcs per verificare a terminale se è presente il semaforo semaforo (dopo aver fatto un'operazione pesante che ha impiegato 10s) perché ne ho ancora bisogno (ho fatto la semget anche io con la stessa chiave)

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     

[PROGRAMMA 2]Ottengo il valore di sem_val per il semaforo con id_sem = 9 (-1 indica che il semaforo non esiste): -1
FINE 2
FINE 1
```

<!-- @todo inserisci qui tutte le note importanti sulla logica -->