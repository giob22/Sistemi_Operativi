# programmazione concorrente

insieme delle tecniche, metodologie e strumenti che gestiscono l'esecuzione di applicazioni concorrenti.

tali applicazioni concorrenti non sono altro che piú attivitá che lavorano in modo simultaneo

La multiprogrammazione é la forma primitiva della programmazione concorrente, infatti mette le basi per la programmazione concorrente, se non fosse implementata non potremmo gestire la sincronizzazione tra i processi interagenti di applicazioni concorrenti.

La multiprogrammazione ci consente di estendere le potenzialitá di una macchina mono processore. É il meccanismo di base che permette l'astrazione di processori virtuali assegnati ad ogni singolo task attivo nel sistema.

Nella programmazione concorrente é onere del programmatore gestire la sincronizzazione delle attivitá che possono essere eseguite simultaneamente ad altre attivitá della stessa applicazione.

Si usa il grafo delle precedenze, a seconda dell'ordinamento di questo possiamo capire se una applicazione concorrente supporta la programmazione concorrente o meno. 

Se il grafo ha un ordinamento totale allora tutte le operazioni di una applicazione devono esser eseguite in modo sequenzuiale, perché ognuna dipende dall'esecuzione di tutte le operazioni precedenti.

Invece nel caso di un ordinamento parziale, significa che le operazioni che non dipendono strettamente dall'esecuzione di tutte le operazioni precedenti, possono essere eseguite in modo simultaneo da piú attivitá.

Queste attivitá quindi possono svolgere simultaneamente operazioni che non hanno un vincolo di precedenza rispetto ad altre operazioni che saranno svolta da un'altra attivitá.

il grafo delle precedenze é composto da nodi che corrispondono ad eventi e da archi de indicano l'ordine con cui le operazioni di ogni nodo devono esser eseguite per ottenere un risultato coerente.

É un modo per rappresentare il flusso di esecuzione di un task in modo formale cosí da identificare quali sono le porzioni dell'esecuzione che possono essser svolte in modo simultaneo

Ogni evento corrisponde all'esecuzione di una operazione.

- processi concorrenti
  
  Insieme di processi la cui esecuzione di sovrappone nel tempo. Ovvero due processi sono concorrente se l'esecuzione della prima operazione di un processo inizia prima dell'ultima operazione dell'esecuzione del secondo processo.

  Quindi se la loro esecuzione é intercalata sul processore allora sono processi concorrenti.

## concorrenza vs parallelismo

concorrenza e parallelismo non sono la stessa cosa, ma sono due tecniche che permettono di velocizzare l'esecuzione di task


## race condition

É un fenomento di interferenza e si manifesta quando piú processi accedono ad una memoria condivisa non in mutua esclusione.

Il principale motivo dovuto alle iterferenze é che le operazioni che vengono eseguite non sono atomiche, quindi sono divisibili. Questo significa che un processo potrebbe esser prelazionato nel durante l'esecuzione di una operazione su una risorsa codivisa che potrebbe esser modificata nel frattempo da un altro processo.

Quindi quando il processo che stava operando sulla risorsa condivisa ritorna ad eseguire non terrá conto del nuovo valore della risorsa, ma lavora su un valore obsoleto.

La race condition si manifesta quando il risultato di una applicazione concorrente dipende dalla velocitá relativa di esecuzione, ovvero da come i processi vengono intercalati nel tempo sul processore.

La velocitá relativa di esecuzione indica l'andamento dell'esecuzione di un processo rispetto all'esecuzione di un altro prociesso.

Ovvero dipende dall'ordine globale con cui il processore esegue le singole istruzioni dei processi che operano sulla risorsa condivisa.

- processi indipendenti
  
  sono i processi il cui flusso di esecuzione non é influenzato dal flusso di esecuzione di nessun altro processo.

- processi interagenti
  
  sono i processi il cui flusso di esecuzione é influenzato dal flusso di esecuzione di altri processi e viceversa.

  Sono quindi processi correlati, con un grado di sincronizzazione.

## Tipi di interazione

- competizione
  
  quando piú processi tentano di accedere ad una risorsa il cui accesso deve essere gestito in mutua esclusione. Quindi un solo processo per volta puó operare su questa risorsa.

- cooperazione
  
  quando tra processi avviene uno scambio di informazioni che permette di definire un ordine nell'esecuzione di alcune operazioni.

  L'esempio piú ovvio di una interazione di tipo cooperazione é il paradigma produttori consumatori, in cui i produttori attraverso segnali indicano ai consumatori che possono eseguire l'operazioni e di consumazione e in modo complementare il viceversa.


- interferenza
  
  erronea soluzione di una applicazione concorrente dovuta ad una mal gestione della cooperazione o sincronizzazione.

Per gestire quindi queste tipologie di interazione nel modo corrette é necessario imporre dei vincoli di sincronizzazione nell'esecuzione dei processi interagenti.

I vincoli per la sincronizzazione riducono la velocitá di esecuzione dei processi ma permettono di ottenere sempre il risultato desiderato. É un giusto compromesso per ottenere la correttezza dei risultati.

Abbiamo due modelli per la sincronizzazione di una applicazione concorrente:

- modello ad ambiente locale
- modello ad ambiente globale 
  
si distinguono sul livello di accoppiamento dei processi interagenti che ne fanno parte.


## Risorsa

É un oggetto fisico o logico che i processi sfruttano durante la loro esecuzione

Una risorsa é rappresentata da procedure e strutture dati, tali procedure implementano delle operazioni sulla struttura dati.

Una risorsa puó essere locale, quindi visibile al singolo processo, oppure globale, quindi condivisa e accessibile da piú processi.

Le risorse su cui operano in maniera concorrente i vari processi devono essere protette da un gestore che sincronizza gli accessi.

Il gestore piú essere esso stesso una risorsa condivisa, oppure puó essere un processo che ha in possesso la risorsa e espleta le operazioni richieste dai vari processi che vogliono utilizzarla.

A seconda del modello che stiamo utilizzando, ambiente locale o ambiente globale, utilizzeremo come gestore un risorsa o un processo.

Entrambi i gestori quindi hanno il compito di essere intermediari delle operazioni che i vari processi possono fare sulle risorse.

Gli strumenti per la sincronizzazione sono forniti dal kernel e permettono di implementare sia un ambiente locale sia un ambiente globale per il controllo della sincronizzazione tra processi interagenti.

## gestore

il gestore di una risorsa ha il compito di garantire un accesso sicuro, senza interferenze, e garantito ai processi

i problemi principali da dover risolvere sono:

- starvation
- deadlock
- mutua esclusione 

la parte di codice in cui un processu utilizza una risorsa in mutua esclusione é detta sequenza critica e deve esser vista dal punto di vista degli altri processi che operano sulla stessa risorsa come una sequenza di operazioni atomica, ovvero deve essere eseguita in maniera sequenziiale rispetto l'esecuzione delle operazioni degli altri processi.


Starvation  = attesa indefinita per processi a bassa prioritá che non riescono ad accedere immediantamente alla risorsa perché utilizzata da altri processi a prioritá maggiore.

il duo accesso alla risorsa non é predicibile nel tempo.

