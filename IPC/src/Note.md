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

Questo quindi vuol dire che nel momento in cui un'altro processo che ha ottenuto direttamente il costruttore (tramite la `shm`) prova ad utilizzare il semaforo potrebbe ottenere un comportamento inaspettato.


