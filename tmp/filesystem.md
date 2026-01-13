# filesystem

Conoscere il filesystem é fondamentale per la sicurezza informatica.

Inoltre é un fatto importante per le prestazioni di un sistema, perché é il componente che gestisce i metodi di accesso alla memoria e quindi anche l'organizzazione della memoria secondaria.

Una corretta organizzazione della memoria secondaria porta alle operazioni di lettura e scrittura su questa ad essere molto piú efficienti.

il file system é responsabile della **gestione e l'organizzazione** dei file su storage.

Gli obiettivi di un FS sono quelli di garantire:

- persistenza
- recupero
- uno schema di naming
- condivisione e la protezione

fornendo un interfaccia unica per la gestione dei file da parte delle applicazioni, mediante l'utilizzo di chiamate di sistema.

- associa nomi che identificano file di dati
- gestisce i dati e i metadati di un file
- verifica i permessi di accesso ad un file
- legge e scrive i metadati su disco
- alloca ed accede ai blocchi su disco

il file é la prima astrazione realizzata da un FS:

- insieme di informazioni raggruppate e memorizzate su disco a cui é assegnato un identificativo simbolico

Insieme ad ogni gruppo di informazioni memorizzate e sul disco é associata una struttura dati mantenuta dal kernel per gestire i permessi e il recupero dei dati contenuti.

Questa astrazione permette ai dati memorizzati in un file, che magari sono sparsi in memoria secondare, di esser visti dalle applicazioni utenti come un vettore di byte.

La directory é la seconda principale astrazione dei FS, viene utilizzata per raggruppare piú file.

Vengono trattate dal sistema operativo come file speciali

Infatti non contengono dati, ma puntatori ai file

Ad ogni file contenuto in una directory é associata una directory entry contenente il nome del file e il riferimento al file su disco.

Il FS per tenenr conto dell'organizzazione del disco utilizza una struttura ad albero contente file e directory.

Ogni directory contiene automaticamente due entries speciali che sono . e .. corrisposndenti al riferimento alla directory corrente e alla directory padre.

Con il comando `ls -l` possiamo avere alcune informazioni sui file contenuti all'interno della cartella in cui stiamo lavorando. 

In particolar modo possiamo vedere la dimensione di ogni file, il numero di riferimenti.

La dimensione di una directory é solitamente di `4KB` espandibili. Quindi una directory normalmente puó esser mappata in una singola pagina della memoria centrale.

Molti sistemi permettono a piú directory di condividere lo stesso file o sotto-directory mediante il meccanismo dei link.

In questo caso la struttura ad albero mantenuta dal FS per gestire l'organizzazione dei file e directory puó contenere dei cicli.

## metodi di accesso

Le operazioni che il FS gestisce sui file sono: 

- creazione
- lettura
- cancellazione 
- apertura
- scrittura 
- chiusura
- riposizionamento

Molti FS forniscono una interfaccia conforme allo standard POSIX in modo da essere piú compatibili possibili.

In modo che piú sistemi operativi possano accedere alla stessa memoria secondaria senza problemi

Infatti le chiavette USB utilizzano un file system POSIX complaier in modo che piú sistemi operativi possibili possano accedere a tale dispositivo.

Accesso sequenziale

per ogni file aperto, il FS ha un puntatore all'ultima posizione acceduta. Dopo un'operazione di lettura o scrittura, il puntatore avanza al byte successivo.

Accesso diretto

Il processo seleziona una posizione arbitraria da accedere, che sará letta o scritta alla prossima operazione

## FCB

la directory entry punta ad un FILE CONTROL BLOCK

É una struttura dati con le informazioni sul file, che quindi definisce l'astrazione di file. In linux la struttura utilizzata é l'inode.

All'interno del FCB sono contenute tutte le informazioni necessarie per la gestione e l'accesso al file su disco.

NOTA: il nome del file non fa parte del dello FCB perché é una informazione mantenuta dal file system all'interno delle entry che rappresentano un file. E serve unicamente al file system per identificare un file o una directory all'interno della struttura ad albero che utilizza per organizzare file e directory.

Per accedere ad un percorso, path, il FS: percorre tutte le directory e i FCB fino ad arrivare ai blocchi su disco del file.

## organizzazione del file

Dal punto di vista del sistema operativo il disco non é altro che un vettore lineare di blocchi.

Ogni blocco puó includere uno o piú settori del disco contigui.

L'unitá di gestione del file system é il **blocco**, che in genere ha una dimensione maggiore del settore del disco.
 
Ogni blocco contiene un insieme di settori contigui

Tipicamente la dimensione di un blocco é pari a quella di una pagina in memoria centrale, per avere in questo modo un mapping 1:1 tra unitá di gestione del disco e unitá di gestione della memoria principale.

Avere un blocco abbastanza grande da contenere i dati corrispondenti ad un intero file migliora i tempi di accesso perché non ho la necessitá di spostare troppe volte la testina, quindi si evitano molti tempi di seek che potrebbero rallentare l'operazione di lettura o scrittura su disco.

Un disco puó esser diviso in partizioni ognuna con un proprio file system.

Oltre alle varie partizioni un disco conterrá due porzioni importanti che vengono memorizzate nei blocchi iniziali che sono:

- Master boot record
- tabella delle partizioni

Il master boot record contiene un programma di avviamento, il bootloader

Il BIOS all'atto dell'avviamento del sistema lancia l'esecuzione del bootloader che a sua volta permette di eseguire il kernel contenuto in una delle partizioni tra cui é diviso il disco.

## metodi di allocazione

i metodi di allocazione definiscono i modi con cui i blocchi sono assegnati ai file

- allocazione contigua
  
  ciascun file occupa un insieme di blocchi contigui sul disco

  il vantaggio é che viene favorito l'accesso sequenziale, per accedere ad un file é necessario conoscere unicamente la posizione iniziale e la grandezza del file.

  gli svantaggi sono che abbiamo frammentazione esternae i file sono difficilmente flessibili ovvero non sempre é possibile aumentare lo spazio dedicato per un file,

  La frammentazione esterna constringe dopo un certo periodo di utilizzo di deframmentare il disco, operazione che consiste nello riorganizzare il disco in modo che tutta la quantitá di memoria persa in forma di spezzono di blocchi possa esser utilizzata.
- allocazione concatenata
  
  Ciascun file é mappato tramite una linked list di blocchi fisici. Quindi ogni blocco conterrá un puntatore al blocco successivo.

  Mi permette di evitare la frammentazione esterna a svantaggio degli accessi random, inoltre ho meno spazio per memorizzare dati all'interno di un blocco, perché una porzione del blocco sará utilizzata per memorizzare il puntatore la blocco successivo che mappa il file.

  inoltre abbiamo una maggiore flessibilitá nella dimensione dei file, un file potrebbe crescere di dimensione molto facilmente.

  Non favorisce l'accesso random, perché per arrivare a valutare uno specifico byte contenuto nel file devo scorrere la linked list.

  Gli accessi sequenziali sono comunque avvantaggiati nonostante per ogni blocco é necessaria una operazione di seek per arrivare al blocco successivo.

  Lo svantaggio consiste nel perdere memoria per contenere dati in ogni blocco perché viene utilizzata per memorizzare il puntatore al blocco successivo

  Possiamo perdere tutti i blocchi successivi ad un blocco se il puntatore contenute in quest'ultimo viene danneggiato.

  - FAT
    
    é un file system che utilizza un metodo di allocazione che é una variante dell'allocazione concatenata.

    Offre una interfaccia POSIX complaier quindi é possibile utilizzarlo sulla maggior parte dei sistemi operativi.

    Oggi infatti é una lingua franca per dischi acceduti da piú sistemi operativi

- allocazione indicizzata
  
  per ogni file raggruppa tutti i puntatori in un unico blocco detto index node

  blocco indice

  Quindi per accedere ad un file facciamo riferimento al blocco indice in cui saranno contenuti i riferimenti a tutti i blocchi fisici che mappano il file.

  Vantaggi sono 

  - non c'é frammentazione esterna
  - maggiore flessibilitá, il file puó crescere molto facilmente
  - favorisce l'accesso diretto
  - maggiore affidabilitá perché il blocco indice puó essere ridondante
  Gli svantaggi sono

  - consumo di spazio per i blocchi indice
  - nel caso di file molto grandi i blocchi indice potrebbero essere diversi, quindi c'é bisogno di genstirli mediante una struttura dati, oppure utilizzando indici di indici

## gestione dello spazio libero

In ogni partizione é dedicata una porzione per mantenere una struttura dati utile al FS per identificare quali siano i blocchi per memorizzare dati liberi o allocati.

Questa struttura dati potrebbe essere ad esempio una bit map, la cui dimensione dipende dalla grandezza di ogni blocco che tipicamente é pari alla grandezza di una pagina, e dalla quantitá di blocchi che vengono utilizzati per memorizzare dati.

ad esempio 1GB di partizione con blocchi di 4KB é necessario avere una bitmap di 32KB.

Oltre alla struttura per mantenere un'informazione sui blocchi liberi all'interno della partizione sono contenute altre porzioni che sono chiamate boot block e super block.

Altri approcci per gestire lo spazio libero potrebbero essere quello di utilizzare:

- una lista concatenata di blocchi liberi, in cui all'interno di ogni blocco libero é memorizzato il riferimento al prossimo blocco libero.
- un raggruppamento: consiste in una variante della lista concatenata dove ogni blocco contiene n puntatori a blocchi liberi e solo l'ultimo puntatore punta ad un blocco contenente altri puntatori a blocchi liberi
- Conteggio: consiste nel memorizzare una struttura in cui vengono memorizzati sequenze di blocchi contigui. Quindi viene memorizzato il riferimento al primo blocco contiguo e la quantitá di blocchi successivi che sono liberi.



