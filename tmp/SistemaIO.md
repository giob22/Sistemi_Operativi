# Sistema IO

Il sistema IO é spesso collo di bottiglia per via delle dinamiche lente delle periferiche di IO in confronto di quelle del processore.

Il sistema di IO del SO fornisce una astrazione dei dispositivi periferici ai programmi utente

- i processi utente accedono ad una vasta eterogeneitá di dispositivi periferisci mediante la stessa interfaccia.
- Quindi l'interfaccia con cui questi accedono é indipendente dalla teconologia dei dispositivi.

Il sistema di io ottiene la richiesta di una operazione di io da parte di un processo per un determinato device io mediante, quindi, l'interfaccia indipendente dal dispositivo di IO che definisce.

Successivamente identifica il dispositivo di IO rispetto al quale il processo sta richiedento l'operazione e avvia il device drive associato a tale dispositivo.

Eseguendo del device driver la funzione corrispondente all'operazione specificata dal processo utente.

I device drivers sono specifici per il dispositivo, quindi sono di difficile implementazione e vengono inseriti all'interno di moduli caricabili dal kernel. 

In modo che questi vengano caricati solo nel caso sia effettivamente possibile comunicare con lo specifico dispositico.

Infatti se il dispositivo non é collegato sarebbe inutile appesantire l'immagine del kernel caricando il modulo contenente il driver.

I driver rappresentano il 70% del codice di Linux e sono tra le maggiori cause di malfunzionamenti.

Il motivo dei malfunzionamenti é dovuto al fatto che é difficile testarli in situazioni di malfunzionamento o guasto del dispositivo. Per farlo dovremmo rompere il dispositivo.

Ci sono peró tecniche che permettono di indurre una guasto sinteticamente, ovvero foult injection.

I driver possono essere eseguiti sia in kernel mode che in user mode. Dipende dall'architettura del sistema operativo.

Nel caso di sistemi operativi monolitici allora viene eseguito in kernel mode.

Nel caso di sistemi operativi a microkernel viene eseguito in user mode da processi di sistema chiamati server.

Nel caso di linux i driver vengono eseguiti nel contesto del processo utente, con un cambio di modo in kernel mode.

Sfruttano quindi la porzione dello spazio di indirizzamento virtuale di ogni processo dedicato al kernel.

Una funzione del driver é quella di ISR per il determinato dispositivo su cui opera.

In questo caso abbiamo maggiori performance ma meno sicurezza perché un malfunzionamento consisterebbe in un eccezione in kernel mode la quale potrebbe portare al sistema un crash

Invece nei sistemi a microkernel si hanno meno prestazioni per via di molti cambi di contesto necessari ma maggiore sicurezza perché il driver esegue in user mode, quindi un malfunzionamento farebbe terminare unicamente il processo server che gestisce il determinato device.

Con l'utilizzo delle interrupt si ha la possibilitá di utilizzare la multiprogrammazione. Ovvero sfruttare l'intervallo di tempo in cui il processo non é in esecuzione, perché é in attesa della terminazione di un operazione di IO, per eseguire altro codice utile, di un altro processo.

spooling é la tecnica che mi permette di sfruttare il disco come area di appoggio per serializzare l'uso di un dispositivo lento e non condivisibile, permettendo cosí ai processi che lo utilizzano di non esser rallentanti.

Quindi una volta che vengono trasferiti tutti i dati nel disco nell'area dedicata allo spooling il processo é illuso di aver terminato l'operazione con il dispositivo lento quindi continua la propria esecuzione; quando in realtá un altro processo speciale sta facendo il lavoro al posto suo.

Questo serve per rendere l'esecuzione dei processi piú veloce nonostante debbano operare su dispositivi molto lenti.

## dischi

Parametri prestazionali per i dischi:

- tempo di seek
  
  Tempo di ricerca per posizionare la testina sulla traccia desiderata.
- ritardo di rotazione
  
  Tempo necessario affinché l'inizio del dato routi presso la testina
- tempo di trasferimento
  
  tempo necessario alla rotazione sull'area del dato

Su questi parametri possiamo realizzare lo scheduling per le richieste di IO

Combinando tutti i parametri ottengo un tempo di accesso medio al dato.

Per calcolare efficientemente il tempo di accesso effettivo ad un dato ho bisogno di conoscere come sono posizionati i dati nel disco. Se il file che intendo leggere é memorizzato con una organizzazione **sequenziale** o con una **random**.

## buffering e caching

Buffer-cache:

Sono delle aree di memoria in memoria principale che **mantengono una copia dei dati che transitano da o verso il disco**.

Mi permetto di **velocizzare dal punto di vista del processo l'accesso al disco**.

Buffer: **lavora sul trasferimento di dati**

- i dati sono prima copiati in memoria principale e poi successivamente scritti in memoria di massa
- mi permette di posticipare la scrittura su disco

Cache: **lavora sul riutilizzo dei dati**

- conserva una copia dei dati del disco in RAM
- In modo che una seconda lettura, il processo, accede alla RAM invece che al disco.

Politiche di IO buffering

**Write-through**: i dati vengono scritti sia su buffer sia su disco immediatamente ad ogni modifica.

**Write-back**: i dati non sono scritti subito su disco, ma svuotati periodicamente o quando si accumulano; Operazione di FLUSH

La politica write-back ha migliori prestazioni, ma puó causare perdita di dati in caso di guasti. Perché nel momento del guasto non sappiamo che il buffer abbia fatto il flush.

Tipicamente tutta la memoria residua viene dedicata la page caching

possiamo modificare i parametri su cui si basa il page caching in linux

- tempo massimo dopo cui un blocco sporco in memoria viene aggiornato sul disco
- percentuale di memoria sporca oltre la quale si forza l'aggiornamento su disco

poi possiamo modifica la Swappiness

Il parametro swappiness regola l'algoritmo di selezione delle pagine vittima per lo swap-out.

- vicino 0: tende a swappare le pagine della cache o del buffer 
- vicino 100: tende a swappare le pagine dei processi, quindi piú facilmente potrebbe esser swappata una pagina facente parte del WS di un processo.

## scheduling

Il SO riceve richieste di IO da piú processi differenti per accedere a file differenti.

Possiamo operare sull'ordine con cui vengono servite le varie richieste in modo da rendere piú efficiente l'utilizzo del disco, e diminuire il tempo medio di risposta oppure il throughput;

Bisogna evitare la starvation e tenta di ottimizzare le prestazioni.

Abbiamo due approcci per le politiche di scheduling

- selezione secondo il richiedente
- selezione secondo l'elemento richiesto
  
  tengono conto della posizione effettiva dell'elemento rispetto la testina, quindi tentano di anticipare le richieste che sono piú vicine in modo da non fare troppi spostamenti superflui con la testina.

FIFO: politica fair con basse prestazioni in presenza di un alto numero di richieste

Quindi potrebbero esser fatti molti movimenti della testina superflui.

PRIORITÁ: come FIFO, ma i processi sono organizzati in code diverse a seconda della loro prioritá

Possibilitá di starvation per processi a bassa priotitá

SHORTEST SERVICE TIME FIRST (SSTF)

seleziona la richiesta che richiede il minimo movimento dei braccetti del disco  rispetto alla posizione corrente

Ottime prestazioni, ma vulnerabile alla starvation, si potrebbero avvantaggiare le richieste che fanno parte dell'intorno in cui si trova la testina in quell'istante

SCAN detto anche algoritmo dell'Ascensore

I braccetti del disco si muovono in un unica direzione, soddisfacendo tutte le richieste pendenti.

Quando arriva all'ultima traccia torna indietro servendo nuovamente le richieste lungo quella direzione

quindi le richieste vengono ordinare in base alla posizione di queste rispetto il movimento del braccetto

la starvation é ridotta ma ancora possibile.

Riduce il **tempo medio di risposta**, evita la starvation anche se possibile, e distribuisce **il tempo di accesso equamente** tra i processi.

Le richieste al centro del disco sono mediamente servite prima rispetto a quelle ai confini

CSCAN é una variante di SCAN, infatti applica l'algoritmo verso un unica direzione,

quando il braccetto arriva all'ultima traccia torna indietro alla prima, quella da cui é partito.

Il tempo di attesa é piú uniforme, migliora la fairness

evita che vengano favorite le tracce centrali

il movimento di ritorno non serve richieste, aumentando la distanza percorsa dalla testina.

Per alcuni workload potrebbe essere piú svantaggioso della politica SCAN

N-step-SCAN

piú code di lunghezza N, ne viene servita una sola per volta con politica scan

Quando una coda é servita le altre vengono riempite di richieste 

FSCAN

due code

le code non hanno un limite numerico

Quando vengono servite con SCAN le richieste di una coda, l'altra coda accumula le nuove richieste.

## scheduling del disco in Linux

Dalla versione 2.4 si utilizza una variante dell'algoritmo CSCAN: Linus Elevator

- IO sorting  ordina le operazioni secondo il blocco richiesto
- IO merging  unifica due operazioni su due blocchi adiacenti

Il merging é il motivo della variante dell'algoritmo di SCAN

Nonostante un elevato throughput, gli sviluppatori di linux hanno riscontrato una forma di starvation: **writes-starving-reads**.

Se la quantitá di scritture é molto superiore alle letture, i processi che hanno richiesto delle letture possono subire starvation.

il motivo principale é dovuto al fatto che le scritture sono bufferizzabili. Quindi un processo potrebbe richiedere tante letture e non sospendersi perché queste vengono scritte in memoria RAM nel BUffer, e con la politica write-back, si permette di posticipare le scritture su disco.

Mentre per le letture non é possibili il buffering, questo perché le letture sono sincrone all'esecuzione dei processi. Quindi sono sempre bloccanti a differenza delle scritture.

Il motivo principale é che il pocesso per continuare la sua esecuzione ha bisogno dei dati che sono stati letti dal disco, per questo la lettura é bloccante. il processo riprende ad eseguire unicamente quando  l'operazione di lettura é terminata.

Nella versione 2.6 del kernel linux si é fatta qualche modifica per evitare la starvation dei processi lettori.

Si sono aggiunte alla coda delle richieste altre 2 code: una per le letture, una per le scritture, che sono gestite con una politica FIFO.

Ogni coda ha un tempo massimo di attesa (deadline).

- 500ms per le read
- 5s per le write

Se la deadline scade allora la richiesta viene servita immediatamente.

in questo modo i processi che richiedono una letttura soffrono meno la starvation da parte di molte operazioni di scrittura.

Per attenuare sempre di piú la starvation per uno scrittore che effettua delle letture multiple si é pensato di valutare il think time.

Ovvero per il principio di localitá il lettore che ha appena fatto una lettura per un blocco, frequentemente tenta di leggere un blocco vicino la blocco letto in precedenza.

Peró questa seconda richiesta potrebbe avvenire dopo un periodo di tempo che avrebbe permesso alla testina di spostarsi nuovamente per eseguire un'altra operazione di IO, quindi questo potrebbe generare piú attesa nei confronti del lettore che sta tentando una seconda lettura.

Quindi si é pensato che nel momento in cui viene eseguita un'operazione di lettura, la testina si ferma per un periodo di tempo detto think time. Ovvero il tempo che il processo che ha fatto una lettura elabori il dato. durante questo periodo di tempo ci si aspetta che molto probabilmente il processo eseguirá un'altra operazione di lettura nelle vicinanze del blocco che ha letto precedentemente.

Questa attesa potrebbe causare dei ritardi significativi se si attende per molte volte. Ma per il principio di localitá le performance migliorano. Adesso il problema principale é capire quanto sia il tempo massimo che la testina deve attendere la seconda operazione di lettura da parte del processo. Perché questa potrebbe anche non arrivare mai.


Questo meccanismo vien detto Anticipatory scheduling

Dopo una lettura il disco attende senza servire richieste 
il disco resta inutilizzato per brevi periodi di tempo.

Il tempo che il disco deve attendere prima di poter espletare la prossima richesta di IO che non sia una lettura vicina al blocco che é stato letto precedentemente é un euristica che non sempre puó portare a vantaggi

Perché se si aspetta e la richiesta di lettura non viene inviata durante il tempo di attesa ma subito dopo, allora il disco sta eseguendo un'altra richiesta, la lettura viene comunque ritardata, e l'intervallo di tempo che il disco ha attesa l'operazione di lettura viene perso.

## Budget fair Queueing BFG

## SSD

i dischi a stato solido sono dispositivi di memorizzazione non volatili basati su tecnologia flash

un SSD é fatto di celle di transistor ad accesso random e riprogrammabile.

Rispetto gli HDD abbiamo una maggior robustezza fisica, maggiori prestazioni, risparmio energetico per via del fatto che non dobbiamo muovere alcuna testina.

Sono utilizzati come alternativa agli HDD nei dispositivi portabili e PC.

Oppure come supporto agli HDD per realizzare HDD ibridi in cui la SSD viene utilizzata come cache.

Il contro é che hanno un usure delle celle maggiore rispetto ad un HDD

inoltre nel momento in cui vogliamo leggere un dato dal disco dobbiamo leggere l'intero blocco in cui é contenuto il dato. Quindi questo comporta usura non alla singola cella ma a tutte le celle contenute nello stesso blocco.

quindi in questo caso sono avvantaggiati gli accessi sequenziali perché ci permettono di operare sull'intero blocco.






-




