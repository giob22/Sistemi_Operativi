# Problema dei lettori/scrittori

Come per il problema produttori/consumatori sono presenti due categorie di processi:

- **Lettori**: leggono un messaggio su di una risorsa condivisa(*shm*);
- **Scrittori**: scrivono il messaggio sulla risorsa condivisa

Ciò che differenzia effettivamente il problema lettori/scrittori da consumatori/produttori sono i **vincoli** da rispettare riguardanti la *cooperazione* e *competizione* tra i processi.

**Vincoli**:

1) i processi *lettori* possono accedere **contemporaneamente** alla risorsa;
2) i processi *scrittori*, tra loro, hanno **accesso esclusivo** alla risorsa;
3) i *lettori* e *scrittori* si **escludono mutuamente** dall'uso della risorsa

Non è presente un vincolo di ordinamento (un lettore non deve leggere solo dopo una scrittura) tra i processi lettori e scrittori, ma un vincolo di **mutua esclusione** (tra scrittori) e **cooperazione** (tra lettore e scrittore) sull'utilizzo della `shm`.

In realtà si vede che è necessario gestire anche la mutua esclusione tra processi scrittori che però non riguarda la `shm` ma una variabile necessaria all'implementazione di questo paradigma: variabile che tiene conto del numero corrente di lettori sulla `shm`.

I processi lettori inoltre non consumano la risorsa nel momento in cui la leggono/usufruiscono, come accadeva per i consumatori → non notificano allo scrittore l'avvenuta lettura perchè non c'è il concetto di spazio libero (come avveniva per consumatore/produttore).

Un *side effect* dei lettori/scrittori è la starvation che può mostrarsi in due casi:
1) **starvation degli scrittori**
2) **starvation di entrambi**
   
Essendo le operazioni di lettura e scrittura delle **sezioni critiche** vanno protette entrambe, con l'accortezza che l'operazione di lettura in realtà è protetta solo da processi scrittori: i processi lettori possono operare **simultaneamente** sulla risorsa.

Quindi avremo procedure per "proteggere": operazioni di lettura che saranno `Inizio_Lettura()` e `Fine_Lettura()`, e per operazioni di scrittura che saranno `Inizio_Scrittura()` e `Fine_Scrittura()`

Un processo **lettore attende** solo se la risorsa è **occupata** da un processo **scrittore** e non da altri lettori.

Mentre un processo **scrittore** può accedere alla risorsa solo se questa è **libera**.

Ed è proprio questo vincolo dovuto alla sincronizzazione che può provocare condizione di **attesa indefinita (starvation)** per i processi **scrittori**.

<p align="center">
  <img src="images/Starvation_scrittore.gif" width="250" alt="Animazione dimostrativa">
</p>


- Uno **Scrittore** ogni qual volta inserisce un dato nella `shm` **sovrascrive il dato preesistente**.
- La lettura non distrugge il dato contenuto
- I lettori di **sospendo** se uno scrittore è sulla `shm`, mentre ciò non accade se sulla `shm` sono presenti altri lettori
- Uno scrittore si **sospende** se vi è qualcun altro processo sulla `shm` che sia un altro scrittore o qualsiasi lettore.

Quindi la presenza di un lettore permette solo ad altri lettori di entrare, ma non permette l'ingresso agli scrittori.

Finché nel sistema vi è almeno un lettore attivo lo scrittore subisce la **starvation**.


## Implementazione con starvation degli scrittori

Dobbiamo quindi creare vari processi scrittori e vari processi lettori che condividono un area di memoria comune e che interagiscano con questa secondo i vincoli che abbiamo determinato.

Per tener conto del numero di lettori che contemporaneamente accedono alla `shm` si utilizza una variabile anche essa condivisa che chiamiamo `NUM_LETTORI`.

Solo nel momento in cui `NUM_LETTORI = 0`, gli scrittori possono accedere (uno alla volta) alla risorsa.

Si utilizzano quindi **due semafori**:

- `MUTEXL` per gestire l'accesso alla variabile `NUM_LETTORI` in mutua esclusione, da parte dei lettori (tale semaforo viene inizializzato ad 1);
- `SYNCH` per gestire la mutua esclusione tra i processi **lettori e scrittori** e anche **tra i processi scrittori stessi** (tale semaforo viene inizializzato ad 1).
  
  Gestisce la **cooperazione** tra scrittore/lettore e la competizione tra gli scrittori.

Quindi il processo lettore esegue la funzione `Lettura()` in cui vengono chiamate rispettivamente le funzioni `Inizio_Lettura()` e `Fine_Lettura()`.

```c
void Lettura(int sem){
    Inizio_Lettura(sem);

    // la lettura
    // ...

    Fine_Lettura(sem);
}
```

Di seguito l'implementazione delle funzioni `Inizio_Lettura()` e `Fine_Lettura()`.
```c
void Inizio_Lettura(int sem){
    wait_sem(sem, MUTEXL);
    Num_Lettori++;
    if (Num_Lettori == 1) wait_sem(sem, SYNCH);
    signal_sem(sem,MUTEXL);
}
```
```c
void Fine_Lettura(int sem){
    wait_sem(sem, MUTEXL);
    Num_Lettori--;
    if (Num_Lettori == 0) signal_sem(sem, SYNCH);
    signal_sem(sem, MUTEXL);
}
```

Supponendo non vi siano scrittori attivi, tutti i lettori eseguono `Inizio_Lettura()` senza sospendersi.

Solo il primo lettore farà la `wait_sem()` sul semaforo `SYNCH` in modo da bloccare eventuali scrittori.

Invece supponendo che il buffer sia già occupato da un processo scrittore, allora il primo lettore si sospenderà sul semaforo `SYNCH` e tutti gli altri lettori si sospenderanno sul semaforo `MUTEXL` (perché il processo che possiede la risorsa `Num_Lettori` è sospeso e non ha rilasciato la risorsa).

Infine la funzione `Fine_Lettura()` fa in modo che solo l'ultimo lettore possa riattivare un eventuale scrittore (esso può accedere solo in maniera esclusiva alla risorsa).

Gli scrittori come detto devono accedere in maniera esclusiva alla risorsa `shm` → l'acceso è gestito dalle funzioni `Inizio_Scrittura()` e `Fine_Scrittura()`.

```c
void Inizio_Scrittura(int sem){
    wait_sem(sem,SYNCH);
}
```

```c
void Fine_Scrittura(int sem){
    signal_sem(sem,SYNCH);
}
```

In questo caso il semaforo `SYNCH` gestisce sia la **mutua esclusione tra lettori e scrittori** che la **mutua esclusione tra gli scrittori stessi**.
## Implementazione con starvation di entrambi

Inseriamo un comportamento analogo dei lettori anche agli scrittori, introducendo la variabile condivisa `NUM_LETTORI`.

In questo caso occorrono `4` semafori, tutti inizializzati ad `1`.

- `MUTEXL` per gestire l'accesso alla variabile `NUM_LETTORI` in mutua esclusione, da parte dei lettori;
- `MUTEXS` per gestire l'accesso alla variabile `NUM_SCRITTORI` in mutua esclusione, da parte degli scrittori;
- `MUTEX` per gestire l'accesso alla `shm` in mutua esclusione, da parte degli scrittori
- `SYNCH` per garantire la mutua esclusione tra i processi lettori e scrittori
  
  In questa implementazione tale semaforo non è più utilizzato per gestire sia la mutua esclusione tra scrittori e lettori che la mutua esclusione tra gli scrittori,
  
  ma gestisce unicamente la **mutua esclusione** tra **lettori** e **scrittori**.

L'implementazione di `Inizio_Lettura()` e `Fine_Lettura()` in questo caso non varia; a variare è l'implementazione di `Inizio_Scrittura()` e `Fine_Scrittura()`.

```c
void Inizio_Scrittura(int sem){
    wait_sem(sem, MUTEXS);
    Num_Scrittori++;
    if (Num_Scrittori == 1) wait_sem(sem, SYNCH);
    signal_sem(sem, MUTEXS);

    wait_sem(sem, MUTEX);
}
```

- Effettua una `wait_sem()` su `MUTEXS` per modificare `Num_Scrittori` in mutua esclusione;
- Se il processo è il primo scrittore allora esegue la `wait_sem()` su `SYNCH` per bloccare l'accesso ai lettori;
- Rilascio della variabile `Num_Scrittori` con una `signal_sem()` su `MUTEXS` così che altri scrittori possano accedervi;
- Infine il processo fa una `wait_sem()` su `MUTEX` affinchè altri scrittori non possano accedere alla `shm`.

```c
void Fine_Scrittura(int sem){
    signal_sem(sem, MUTEX);
    
    wait_sem(sem, MUTEXS);
    Num_Scrittori--;
    if (Num_Scrittori == 0) signal_sem(sem, SYNCH);
    signal_sem(sem, MUTEXS);
}
```

- Con `signal_sem()` su `MUTEX` permetto ad un altro scrittore sospeso sulla `shm` di accedervi;
- successivamente con un `wait_sem()` su `MUTEXS` il processo accede alla variabile `Num_Scrittori` in mutua esclusione;
- Se tale variabile è pari a `0` allota il processo è l'ultimo scrittore, quindi effettua una `signal_sem()` su `SYNCH` in modo da permettere l'accesso ai lettori;
- infine il processo rilascial la variabile `Num_Scrittori` attraverso una `signal_sem()` su `MUTEXS` affinchè altri scrittori possano accedervi.

Da notare che l'implementazione di queste due funzioni è l'analogo delle funzioni per inizio e fine lettura per i lettori. L'unica differenza sta nel fatto che per gli scrittori è da gestire la loro mutua esclusione sulla `shm` che non è necessario per i lettori.

A questo proposito infatti è impiegato il semaforo identificato all'interno del *semaphor set* dalla costante `MUTEX`.


## Implementazione con MONITOR

<!-- @todo fai l'implementazione di inizio e fine scrittura e lettura sfruttando un monitor signal-and-wait ed uno signal-and-continue-->

