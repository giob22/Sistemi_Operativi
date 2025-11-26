# Gestione della memoria

La **memoria principale** costituisce, insieme la CPU, una delle risorse per realizzare **l'astrazione di processo**.

Il processo dispone di una **area di memoria** ad esso **riservata** (non accessibile da altri processi).\
Tale area di memoria (*memoria virtuale*) illude il processo facendogli credere di avere a disposizione **l'intera memoria principale**.

Questa memoria riservata permette a ciascun processo di avere una **vista indipendente** **e continua** della memoria, anche se, fisicamente, la memoria principale è condivisa tra più processi e l'allocazione dell'immagine di questi non è detto che sia continua.

Il sistema operativo, come per il processore, attraverso l'utilizzo dei **PCB** garantisce il principio di **isolamento** tra processi, ovvero è necessario assicurarsi che l'esecuzione di uno non interferisca con quella di un altro.\
Quindi fisicamente i processi non devono accedere ad aree di memoria fisiche in cui è contenuta l'immagine di un altro processo.

<p align='center'><img src='images/immagine_allocata_in_RAM.png' width='500' ></p>

L'immagine del processo è descritta all'interno del PCB dello specifico processo.\
In questa struttura sono contenute anche informazioni **su dove si trovano** le diverse parti dell'immagine **in memoria centrale**.

I processi non accedono mai direttamente alla memoria principale tramite degli indirizzi.\
Lavorano invece su una propria **memoria virtuale** (riservata), esso non utilizza indirizzi di memoria fisici ma bensì **virtuali**.

Tali **indirizzi virtuali** hanno senso solo nella visione della memoria del processo → non corrispondono a veri e propri indirizzi della memoria principale.

---

ESEMPIO:

Quando utilizziamo indirizzi all'interno di un codice non stiamo realmente utilizzando indirizzi fisici ma indirizzi virtuali.

Infatti quando stampiamo gli indirizzi di memoria delle variabili che utilizza un processo non stiamo realmente stampando indirizzi di memoria, ma indirizzi di memoria logici.

```c
#include <stdio.h>
#include <stdlib.h>
int main() {
    int x = 3;
    printf("location of stack: %p\n", &x);
    char * p = malloc(1024);
    printf("location of heap : %p\n", p);
    printf("location of code : %p\n", main);
}
```

```console
location of stack: 0x7fff691aea64
location of heap : 0x1096008c0
location of code : 0x1095afe50
```

Tali indirizzi virtuali che abbiamo stampato vengono tradotti da un componente hardware (MMU - Memory Management Unit) per eseguire le istruzioni desiderate.

---

Il kernel a differenza di un Hypervisor astrae l'hardware per consentire l'esecuzione di un processo in modo indipendente.

La posizione (**indirizzi**) di codice e dati nella memoria di un processo è un'**astrazione**. → indirizzi virtuali.

La posizione effettiva in memoria fisica è **gestita dal sistema operativo**.

## Aspetti e parametri caratterizzanti la gestione della memoria 

- **Supporto Hardware** per la gestione della memoria, in particolare l'isolamento tra i processi. (MMU)
- **Organizzazione logica** della memoria virtuale, ovvero le diverse sezioni che fanno parte della memoria virtuale di ogni processo.
- **Organizzazione fisica** della memoria principale, ovvero descrive come è allocata in memoria principale l'immagine di ogni singolo processo.
  
  Tale organizzazione è necessaria per capire dall'indirizzo virtuale a quale indirizzo fisico si sta effettivamente accedendo.
- **Dimensione della memoria virtuale** che può essere più **grande della memoria fisica disponibile**. 
  
  Quindi il sistema operativo deve fare in modo di emulare la memoria fisica utilizzando la memoria secondaria, per permettere l'esecuzione dei processi che utilizzano più memoria di quanta ne sia disponibile.
- **Rilocazione**:
  
  La rilocazione è un meccanismo che permette di caricare l'immagine di un processo in memoria principale.

  Esistono due principali tipi di rilocazione:

  - statica: gli indirizzi vengono definiti al momento della compilazione o caricamento del programma e non possono essere cambiati.
  - dinamica: gli indirizzi possono essere modificati durante l'esecuzione del programma, ad esempio quando il programma viene riattivato e ritorna nella memoria principale dallo swap.
- **Organizzazione dello spazio virtuale**
  
  - spazio virtuale unico: tutta la memoria di un processo è trattata come un'unica area.
  - spazio virtuale segmentato: la memoria del processo è suddivisa in **segmenti** (ad esempio, codice, dati, stack) che vengono gestiti separatamente.
- **Allocazione**
  
  Si riferisce alla maniera con cui il sistema operativo assegna la memoria principale ai processi.

  - contigua: i dati di un processo sono collocati in un blocco contiguo di memoria.
  - non contigua: i dati di un processo possono essere sparsi in diverse aree della memoria, come avviene nella **paginazione**.
- **Caricamento**
  
  Il caricamento riguarda come i processi vengono caricati in memoria:

  - tutto insieme: il processo viene caricato tutto in una volta nella memoria.
  - a domanda: solo le parti del processo necessarie vengono caricate in memoria, quando l'esecuzione ne richiede l'accesso.

## Tecniche di gestione della memoria

---

Rilocazione significa allocare l'immagine del processo nella memoria principale, può avvenire in diversi modi.

Questa rilocazione può essere statica o dinamica.

---

Nei primi sistemi, il posizionamento del codice e dati è **fisso**. → con asim abbimo inserito in posizioni fisse il codice e i dati.\
Il processo è caricato in RAM tutto insieme come un **unico blocco** (allocazione contigua).

<p align='center'><img src='images/primi_sistemi_gestione_memoria.png' width='600' ></p>

Statica perché era più semplice.

Con l'introduzione di hw più evoluti si è permesso di configurare nella CPU il posizionamento di codice e dati, a **tempo di esecuzione**.\
→ ad esempio sfruttando una base e un offset per determinare l'indirizzo fisico.

Con il supporto dei compilatori, la memoria del processo è divisa in blocchi (segmenti) che possono essere **gestiti in maniera separata e indipendente dal SO**.\
Ad esempio il SO protegge il segmento dedicato al codice dando il permesso di sola lettura all'area di memoria a cui sarà destinato.

<p align='center'><img src='images/evoluzione_hw.png' width='600' ></p>

Ulteriori evoluzioni hw hanno permesso di **caricare** l'immagine del processo (o un suo segmento) **non più per intero**, ma in **piccole porzioni (pagine)** ( caricamento a domanda) **sparpagliate** in memoria fisica (allocazione non contigua).

Quindi è stata introdotta l'allocazione di tipo **non contigua** grazie allo sviluppo dell'hardware MMU.

Che vantaggio mi da l'accolazione di tipo non contigua?\
→ uso efficiente dell'area di memoria in termini di **flessibilità e dinamicità**; i processi non devono essere necessariamente allocati in un blocco contiguo di memoria.\
→ attenua il **problema della frammentazione**, perché non ho bisogno di trovare una area di memoria di dimensione tale da contenere interamente l'immagine del processo. Ci sono due tipi principali di frammentazione:

- frammentazione interna: si verifica quando la memoria allocata per il processo **è più grande di quella effettivamente necessaria**.
- frammentazione esterna: si verifica quando ci sono **molti piccoli spazi liberi sparsi** nella memoria fisica, ma **nessuno** di questi **è abbastanza grande da ospitare un nuovo processo**.
  

<p align='center'><img src='images/introduzione_MMU.png' width='600' ></p>


- **Partizioni fisse**: memoria suddivisa in blocchi di **dimensione fissa**. Facile da gestire ma potrebbe causare **frammentazione interna**.
- **Partizioni variabili**: la memoria virtuale viene allocata in blocchi di **dimensioni variabili** a seconda delle necessità del processo. Riduce la frammentazione interna ma può comunque portare a **frammentazione esterna**.
- **Partizioni multiple**: una combinazione di partizioni fisse e variabili.
- **Segmentazione a domanda**: caricamento dinamico di segmenti di processi solo quando necessari. Ottimizza l'uso della memoria, ma può causare ritardi nei processi se la memoria virtuale deve essere frequentemente caricata dal disco.
<!-- @todo aggiungi paginazione e segmentazione con paginazione -->

## Rilocazione 

Modalità di allocazione dei dati e del codice di un processo in memoria fisica.

### statica

La rilocazione statica stabilisce gli indirizzi di codice e dati **al momento della compilazione o caricamento**.

La posizione del codice e dei dati **non può più essere modificata** a tempo di esecuzione.\
→ nella programmazione in assembly si decide a priori dove il codice viene posizionato in memoria centrale.

<p align='center'><img src='images/rilocazione_statica.png' width='550' ></p>

Il problema di questo tipo di rilocazione si presenta nel **contex switch**: il processo quando viene prelazionato e inserito nella coda dei processi pronti oppure swappato\
→nel momento della sua successiva esecuzione deve trovarsi sempre nella stessa area di memoria.\
Questo è un grande **onere computazionale** affidato al sistema operativo.

### dinamica

Nella rilocazione di tipo dinamico si introduce un distinzione fondamentale:

- **Indirizzo virtuale**:
  
  indirizzo **acceduto dal programma** durante l'esecuzione.

- **Indirizzo fisico**:
  
  indirizzo visto dall'unità di memoria, **posizione effettiva** del dato/istruzione.

Quando otteniamo **segmentation foult** significa che l'indirizzo virtuale che stiamo utilizzando, erroneamente, non è associato ad un indirizzo fisico facente parte dell'immagine del processo nella memoria fisica → l'indirizzo fisico che stiamo deferenziando non è mappato nella memoria virtuale.\
→ la traduzione da indirizzo virtuale a indirizzo fisico ha dato questo problema.

Vedremo che il foult può essere **ripristinabile**, perché il segmento/pagina non sono in memoria fisica ma sono swappati in memoria di massa. → ci troviamo nella situazione di un caricamento a domanda.

Il processore vede solo indirizzi logici (virtuali)\
→ **tradotti dall'hardware** che ha la visibilità della memoria fisica, ovvero MMU.\
MMU rende trasparente l'accesso del processore agli indirizzi fisici.

Se non fosse così il processore **sarebbe più performante** (vedrebbe direttamente gli indirizzi fisici), ma perderei tutta la parte di virtualizzazione.

Infatti come abbiamo visto, aprendo il file eseguibile od oggetto, non sono presenti indirizzi fisici ma unicamente indirizzi virtuali che solitamente partono tutti da `0x0`.

<p align='center'><img src='images/rilocazione_dinamica.png' width='550' ></p>

#### Vantaggi della rilocazione dinamica

La rilocazione dinamica consente lo **swapping**
- un processo è temporaneamente **sospeso** e trasferito in **memoria secondaria**;
- il processo potrà poi essere ri-caricato in un'**area** di **memoria** **differente**, in base alla situazione attuale della memoria.

#### MMU (Memory Management Unit)

MMU è un componente hardware della CPU che ha il compito di rendere **trasparente l'accesso al processore** alla memoria fisica tramite una traduzione di **indirizzi virtuali** (utilizzati dal processore) **in indirizzi fisici**.

ESEMPIO DI RILOCAZIONE DINAMICA:

Caso basilare (obsoleto):

- spazio virtuale unico → viene trattato come un unica area.
- allocazione contigua → viene allocato in memoria tutto insieme.

MMU in questo caso possiamo rappresentarla come un componente hardware avente due registri speciali: **limite** e **base**.

In questo caso banale la traduzione avviene con la somma tra l'indirizzo virtuale + la base, ovviamente a valle di una verifica sull'indirizzo virtuale (se esiste un indirizzo fisico mappato con tale indirizzo virtuale).

<p align='center'><img src='images/caso_obsoleto.png' width='550' ></p>

Ovviamente dobbiamo prevedere un modo per modificare i registri dell'MMU, che sono specifici per ogni processo.

Oltre a cambiare valore per ogni processo i due registri devono esser modificati anche ogni volta che un processo passa in esecuzione da che era swappato\
→ con la rilocazione dinamica potrebbe ritrovarsi in una posizione della memoria fisica differente alla precedente

Questo modello infatti non è la realtà, è uno schema di funzionamento base dell'MMU.

## Gestione dello spazio virtuale

Vi sono due possibili approcci per gestire lo **spazio virtuale** degli indirizzi.

- Uno **spazio unico**\
  (corrispondente all'intero processo)
- Un insieme di **segmenti**\
  (**segmentazione** → la memoria del processo è gestita in porzioni separate)

<p align='center'><img src='images/approcci_spazio_virtuale.png' width='500' ></p>

Nell'approccio segmentato l'immagine del processo è suddivisa in **porzioni logiche** (segmenti) gestite in modo separato ed indipendente dal sistema operativo.\
Ogni segmento può avere una dimensione variabile, in base alla struttura logica del programma (codice, heap, stack, dati).

**Vantaggio principale**:\
La segmentazione facilita la condivisione di aree di memoria fisica tra più processi, mappandole in segmenti distinti del loro spazio di indirizzamento.\
* → Esempio:\
  il segmento di codice (text) di un programma è di sola lettura.\
  Più processi che eseguono lo stesso programma possono **condividere la stessa copia fisica** del codice, mappandola nel proprio spazio di indirizzamento virtuale all'interno del segmento dedicato all'area testo.

### segmentazione

<div style="display: flex;">
<div>
A <b>tempo di compilazione</b>, si configura lo spazio virtuale segmentato.

Viene creato un diverso **segmento** per ciascun **modulo** del programma.

---

#### vantaggi della segmentazione

- **Protezione** dei segmenti;\
  permette una **granularità fine** nella gestione dei permessi sullo spazio di indirizzamento.\
  Ad ogni segmento può avere **permessi diversi** (lettura, scrittura, esecuzione).

</div>
<p align='center'><img src='images/segmenti_per_moduli.png' width='300' ></p>
</div>

- **Condivisione** dei segmenti;\
  segmenti come **codice** (text) o le **librerie** **condivise** possono essere mappate da più processi e condividere un'unica copia fisica, **riducendo l'uso della memoria**.

- **Allocazione indipendente** dei segmenti in memoria fisica → riduce (ma non elimina) il problema della frammentazione.\
  Ogni segmento può essere **collocato separatamente in punti diversi** della memoria fisica.

---

Un indirizzo virtuale è una coppia (contiene due informazioni), fatta da:

- un **identificativo** del segmento;
- uno **scostamento** (offset) all'interno del segmento.

<p align='center'><img src='images/info_indirizzi_virtuali.png' width='600' ></p>

L'**MMU** è il componente su cui si basa anche la segmentazione.

- Traduce gli indirizzi virtuali dalla forma (*segmento*, *offset*) in indirizzi fisici.
- Nel caso di **pochi segmenti**, è sufficiente avere nella MMU più coppie di registri base/limite: uno per ogni segmento.

Questo è un **limite** fisico importante perché non mi permetterebbe di avere molti segmenti. → non posso avere un numero infinito di registri.

---

ESEMPIO SEGMENTAZIONE CASO SEMPLICE:

<p align='center'><img src='images/MMU_multi_registro.png' width='500' ></p>

- in questo caso ogni coppia di registri corrisponde a limite e base per un segmento allocato in memoria fisica

---

Nel caso **generale**, quando si vuole supportare un **numero arbitrario di segmenti**, non è possibile avere una coppia **base/limite** nella MMU per ogni segmento di un processo.

Per questo motivo:

- le coppie base/limite non risiedono nella MMU, ma sono memorizzare in **memoria RAM**;
- queste informazioni sono raccolte in una struttura dedicata, chiamata **tabella dei segmenti (segment table)** unica **per ogni processo**;
- ogni **entry** della tabella dei segmenti contiene i dati relativi a un **segmento del processo**: **indirizzo base**, **limite** e **bit di controllo** (permessi rwx).

La MMU gestisce la segment table con due appositi **registri**:

- **STBR** (Segment Table Base Register): indirizzo in memoria fisica in cui si trova la tabella dei segmenti.
- **STLR** (Segment Table Limit Register): dimensione della tabella dei segmenti (indica il **numero di segmenti del processo**)

---

ESEMPIO DI TRADUZIONE UTILIZZANDO LA SEGMENT TABLE:

<p align='center'><img src='images/trad_segment_table.png' width='600' ></p>

- `sg` è l'offset per identificare l'entry point relativo ad un segmento.
- Per accedere alla tabella il processore utilizza le informazioni contenute in `STBR` e `STLR`  i cui valori sono contenuti all'interno del PCB di ogni processo.

---

NOTA sulla segment table:

- ogni processo ha una **segment table differente**
- i registri STBR/STLR sono configurati ad ogni **contex switch** dei processi\
  → durante il contex switch il SO carica i valori di STBR/STLR dal **PCB del processo** (sono legati al singolo processo).

<p align='center'><img src='images/contex_ST.png' width='600' ></p>

#### protezione

Ogni segmento può avere diversi **permessi di accesso** specificati da bit di controllo contenuti nella segment table.

- Ogni riga della segment table di un processo contiene per ogni entry anche una sequenza di **bit di controllo** per gestire i permessi sull'area di memoria in cui è mappato il segmento.
- MMU produce una **exception** se il programma non rispetta i permessi.

<p align='center'><img src='images/entry_bit.png' width='400' ></p>

#### condivisione

La segmentazione consente la **condivisione dei segmenti** tra più processi, allocando in memoria fisica una sola copia del segmento.

Abbiamo due processi con le rispettive tabelle dei segmenti.

→ Se hanno una entry in comune significa che i due processi possono accedere alla stessa area di memoria fisica.\
Quindi stanno effettivamente condividendo la copia del segmento di memoria. Questo permette di allocare una sola copia del segmento in memoria.

<p align='center'><img src='images/condivisione_segmento.png' width='500' ></p>

#### allocazione

Uno spazio/segmento di memoria virtuale può essere **collocato in memoria fisica** in due possibili modi:

- allocazione **contigua**:
  
  lo spazio/segmento è **copiato per intero**, in un intervallo di memoria fisica agli indirizzi [D;D+I]

- allocazione **non contigua** (**paginazione**)


##### allocazione contigua

- Il SO colloca il proprio blocco di memoria virtuale, e quelli dei processi, in intervalli **non-sovrapposti** della memoria fisica
- Quando un processo termina, la memoria fisica occupata si libera, creando un **hole**
- Quando un nuovo processo viene caricato, occorre **cercare un hole sufficientemente grande** da contenerlo
  
  → compito dello sheduler a medio termine.

<p align='center'><img src='images/allocazione_contigua.png' width='600' ></p>

Questo è un esempio su come facilmente si può arrivare ad un problema di **frammentazione esterna** della memoria.

Se ci sono **più buchi liberi**, ci sono vari criteri per scegliere dove collocare un segmento:

- **first-fit**: si assegna il **primo** hole sufficientemente grande;
- **best-fit**: si assegna lo hole **più piccolo** tra quelli sufficientemente grandi per contenere lo spazio di indirizzamento del processo;
- **worst fit**: si assegna lo hole più grande.

In generale, gli **schemi a partizione di dimensione variabile** soffrono del problema della frammentazione esterna.

**Frammentazione esterna**: spazio di memoria perduto sotto forma di spezzoni.

- Lo spazio di memoria totale sarebbe sufficiente per soddisfare una richiesta, **ma non è contiguo**.
- → non si sfrutta a pieno la quantità di memoria totale a disposizione.

Dualmente gli schemi a partizione di dimensione fissa soffrono del problema della frammentazione interna.
<!-- @todo continua -->