# Parallelismo implicito

In questo tipo di parallelismo il codice che viene eseguito è scritto in modo sequenziale, ma sotto il cofano il sistema capisce quali parti sono idipendenti tra loro e le esegue insieme per risparmiare tempo.

Il parallelismo implicito avviene principalmente a due livelli:

- livello **hardware**
  
  Le moderne CPU non eseguono un'operazione per volta, utilizzano una tecnica che gli permette di valutare le prossime istruzioni che dovranno eseguire; questa tecnica è chiamata *Out-of-Order Execution*.

  La CPU quindi analizza le prossime istruzioni che dovrà, se queste sono indipendenti tra loro allora le lancia contemporaneamente su due unità di calcolo diverse.

  Oppure un'altra tecnica hardware è il *Pipelining*: mentre un'istruzione viene eseguita, la successiva viene già decodificata e quella dopo ancora viene prelevata dalla memoria. Quindi si sfrutta il ciclo del processore nell'eseguire singole istruzioni.

- livello **compilatore**
  
  Il compilatore può ottimizzare il programma ancora prima che questo venga eseguito dalla CPU.

## Limini del parallelismo esplicito

- Quantità di codice parallelizzabile all'interno di un programma, questa è una proprietà intrinseca alle operazioni che vengono descritte nel codice, quindi non possiamo metterci mano
- costi dovuti alla sincronizzazione tra i processi che operano su processori diversi
  
  Queste operazioni di sincronizzazioni prevedono di accedere ad un'area della memoria che permetta di gestire la sincronizzazione che è a sua volta condivisa. Quindi, nel momento in cui, un processo accede al gestore della risorsa, che è a sua volta una risorsa condivisa,( per gestire la sincronizzazione) l'operazione che esegue sul gestore dal punto di vista dei processi che eseguono in parallelo su altri processori deve essere un'operazione atomica.

  Per permettere ciò il processore non deve permettere a processi su altri processori di poter accedere all'area di memoria su cui sta lavorando, quindi una tecnica implementabile sarebbe quella di inibire l'accesso al bus PCI alle altre CPU temporaneamente, fin tanto che l'operazione che deve esser vista come atomica non termini.


- costi dovuti alla maggior possibilità di cache miss
  
  La maggior possibilità di cache miss consiste nel fatto che deve sempre essere garantita la cooerenza tra la copia in cache e quella in RAM di un pagina, ma se più processi in parallelo possano accedere alla stessa pagina in memoria fisica, allora la sincronizzazione tra la copia all'interno della singola cache rispetto a quella in RAM deve esser eseguita più spesso.

  Ovvero è molto più probabile che la copia in cache sia una copia obsoleta, perché altri processi che operano su altri processori non hanno a disposizione la stessa memoria cache, quindi una loro modifica su quel frame comporta una invalidità delle copie per tutte le altre cache degli altri processori. Ovviamente se queste cache contenevano una copia del frame.

## Archietetture parallele


MIMD = multiple instructions multiple data

è uno dei modelli fondamentali di calcolo parallelo. Ciascun processore esegue un programma diverso su dati diversi in maniera asincrona.

La comunicazione può avvenire, tra processori, attraverso:

- una memoria condivisa o segnali → strettamente accoppiati
- una rete di interconnessioni → debolmente accoppiati

SMP = simmetric multiprocessor

consiste in un insieme di processori che condividono la stessa memoria fisica, le stesse risorse e sono sotto il controllo dello stesso sistema operativo.

L'organizzazione tipica di un'architettura SMP:

- sono presenti diversi processori
- ogni processore ha accesso ad un unica memoria fisica e risorse condivise.
- i processi possono comunicare tra loro tramite segnali o attraverso porzioni di memoria condivise
- la memoria solitamente è organizzata in modo che sia accessibile simultaneamente a blocchi separati.

Nel caso di architetture multi processor il SO deve gestire:

1) assegnazione dei processi ai processori → distribuzione del carico di lavoro
   
   Ovvero sencondo quale metrica viene assegnato un processo ad un processore e se questo una volta assegnato ad un processore possa esser assegnato successivamente ad un altro durante la sua esecuzione.
2) dispatching dei processi e thread → politica di scheduling da adottare per ogni processore
   
   Ovvero quale metrica si utilizza per decidere quale tra i processi, che sono pronti, assegnati ad un processore deve eseguire sul processore.
3) uso del multitasking su ogni processore → si valuta se per ogni processore i processi possano operare in modo concorrente
   
   Ogni CPU può eseguire in maniera concorrente i processi ad essa assegnati





