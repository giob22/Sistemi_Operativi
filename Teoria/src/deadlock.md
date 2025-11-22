# Deadlock

Deadlock è un fenomeno che può presentarsi nelle **applicazioni concorrenti** che porta a un blocco permanente di processi in **competizione per risorse condivise**.

→ può portare a crash, situazioni di stallo, etc. (da evitare completamente)

Nei sistemi operativi **general-porpose**, si ammette l'esistenza di deadlock → non si implementano tecniche per evitare il problema ma si rileva nel momento in cui si presenta e si cerca di risolverlo.

## Deadlock: definizione e generalità

Indica una situazione di **blocco permanente** di un gruppo di processi in competizione per le risorse di sistema (che sono limitate → motivo principale per cui si deve gestire la competizione).

Deadlock è un problema complesso e di rilievo, che può provocare gravi malfunzionamenti.

A seconda dello scopo per cui è progettato, un sistema operativo adotta una **gestione** diversa per controllare il deadlock.

Ad esempio, per i sistemi **real-time** evitare il deadlock è fondamentale, quindi avranno una gestione più rigida che tende a prevenire tali fenomeni a differenza di altri tipi di sistemi operativi, come general-porpose, che potranno tendere a non rilevarli e risolverli a posteriori.

---

Esempio: attraversamento di un incrocio

<p align='center'><img src='images/incrocio_deadlock.png' width='450' ></p>

Sono presenti diversi quadranti identificati dalle lettere `a`, `b`, `c` e `d`; che possono rappresentare le risorse critiche.

- Ogni auto ha bisogno di attraversare due quadranti;
- Ogni auto rappresenta un processo di un sistema operativo.

Ogni si inserisce nel rispettivo primo quadrante, ma per completare la curva deve ottenere l'accesso al quadrante alla loro destra (o sinistra) occupato da un'altra auto, e per quest'ultima vale lo stesso ragionamento.

Facendo lo stesso ragionamento si crea un situazione di stallo (se nessun'auto indietreggia).

---

Esempio: copia di un file

- un sistema ha 2 dischi esterni
- `P1` e `P2` copiano un grosso file da un disco all'altro;
- si suppone sia necessaria la **mutua esclusione**.

Quindi ciascun processo acquisisce uno dei due dischi per leggere e successivamente richiedono l'accesso all'altro.

Ma l'altro disco si ritrova già acquisito. Quindi i due processi attendono a vicenda che l'altro rilasci la risorse.

<p align='center'><img src='images/esempio_dischi.png' width='350' ></p>

→ si crea la cosiddetta **attesa circolare** che è la manifestazione del deadlock.

---

### Il problema del deadlock

Una condizione **necessaria** affinché nelle **applicazioni concorrenti** si verifichi un deadlock è che siano presenti almeno due semafori **mutex**, ovvero entrambi inizializzati ad 1.

Quindi quando è presente la **mutua esclusione**.

<div style="display:flex; gap:20px;">

  <!-- Colonna P1 -->
<div style="border:2px solid #b7c0e3; background:#eef1ff; padding:15px; width:45%; border-radius:6px; color:#2b2b2b;">
  <b>P₁</b><br><br>
  wait (<span style="color:red;">mutex1</span>)<br>
  &lt;inizio uso disco 1&gt;<br>
  …<br><br>

  <div style="border:2px dashed red; padding:6px; border-radius:4px; color:#2b2b2b;">
    wait (<span style="color:green;">mutex2</span>)
  </div>

  &lt;inizio uso disco 2&gt;<br>
  …<br><br>
  signal (<span style="color:green;">mutex2</span>)<br>
  …<br>
  signal (<span style="color:red;">mutex1</span>)<br>
</div>

  <!-- Colonna P2 -->
<div style="border:2px solid #d0d0d0; background:#f5f5f5; padding:15px; width:45%; border-radius:6px; color:#2b2b2b;">
  <b>P₂</b><br><br>
  wait (<span style="color:green;">mutex2</span>)<br>
  &lt;inizio uso disco 2&gt;<br>
  …<br><br>

  <div style="border:2px dashed red; padding:6px; border-radius:4px; color:#2b2b2b;">
    wait (<span style="color:red;">mutex1</span>)
  </div>

  &lt;inizio uso disco 1&gt;<br>
  …<br><br>
  signal (<span style="color:red;">mutex1</span>)<br>
  …<br>
  signal (<span style="color:green;">mutex2</span>)<br>
</div>

</div>


I due processi potrebbero sospendersi entrambi su le `wait()` evidenziate.

Questo è un classico esempio di attesa circolare che si presenta se avviene la seguente sequenza di azioni:

- `P1` esegue `wait(mutex1)` e acquisisce la prima risorsa critica;
- `P2` esegue `wait(mutex2)` e acquisisce la seconda risorsa critica;
- `P1` ha bisogno della seconda risorsa critica, quindi esegue `wait(mutex2)` e si blocca;
- `P2` ha bisogno della prima risorse critica, quindi esegue `wait(mutex1)` e si blocca.

Nel caso in cui l'esecuzione dei due processi segue questa sequenza, i due processi rimangono bloccati

Questa situazione però **non** **è** **detto** che accada a ogni esecuzione dell'applicazione concorrente.

→ **non** è un fenomeno **deterministico**, ovvero non significa che questo si manifesti ogni volta che si hanno le condizioni adatte.

In alcuni casi il deadlock dipende da come i processi si alternano sulla CPU.

Per questo motivo il deadlock può manifestarsi **saltuariamente**, in base alla **velocità relativa di esecuzione dei processi**.

> La velocità relativa di esecuzione dei processi è un modo per descrivere come i processi avanzano nel tempo l'uno rispetto l'altro, questo avanzamento non è deterministico perché durante l'esecuzione possono:
>
> - essere interrotti dal kernel (preemption),
> - essere sospesi in attesa di un operazione di I/O (la cui durata non è sempre la stessa),
> - ricevere più o meno tempo di CPU in base alla politica di sheduling adottata,
> - essere ritardati da cache miss.

Come si potrebbe risolvere questa situazione?

Un terzo dovrebbe gestire questa concorrenza, andando ad esempio a killare un processo in modo che questo liberi la risorsa detenuta a favore dell'altro.

<div align="center">

## **Deadlock ≠ Starvation**

<table>
<tr>
<td align="center" style="border: none">
  
**attesa <span style="color:red">infinita</span>**

</td>
<td align="center" style="border: none">

**attesa <span style="color:red">indefinita</span>**

</td>
</tr>
</table>

</div>

Deadlock e starvation sono due concetti totalmente diversi che possono essere confusi.

- Con starvation identifichiamo una situazione di attesa **indefinita**. Tale fenomeno è molto legato al concetto di priorità.

- Con deadlock identifichiamo una situazione di attesa **infinita**, in nessun modo i processi possono uscire da questa situazione, al contrario della starvation. Tale fenomeno è molto legato al concetto di mutua esclusione.

## Grafo di assegnazione delle risorse

Il grafo si assegnazione delle risorse serve a modellare in modo formale lo stato delle risorse e dei processi di un sistema, per poter **capire se esiste il rischio di deadlock**.

Tramite questa formalizzazione possiamo implementare algoritmi in grado di rilevare una possibile manifestazione di deadlock tra i processi in esecuzione.

Un grafo è un insieme di vertici (o nodi) *V* e un insieme di archi *E*.

- V è partizionato in **due tipi**:

  - *P* = {P1, P2, ..., Pn} è l'insieme costituito da tutti i **processi** nel sistema.
  - *R* = {R1, R2, ..., Rn} è l'insieme costituito da tutti i tipi di **risorse** nel sistema.
- **Arco di richiesta** (arco orientato) Pi → Rj, Pi chiede l'accesso a Rj.
- **Arco di assegnazione** (arco orientato) Rj → Pi, Rj è assegnata al processo Pi.

Ogni risorsa può avere più istanze.

<p align='center'><img src='images/def_grafo.png' width='300' ></p>

Una condizione **sufficiente** per la possibile (perché dipende sempre dalla velocità relativa di esecuzione) manifestazione di un deadlock è un **ciclo** nel grafo di assegnazione.

<p align='center'><img src='images/grafo_deadlock.png' width='300' ></p>

In questo caso `P1` richiede `Ra` la cui unica istanza è detenuta da `P2` che richiede a sua volta `Rb` la cui unica istanza è detenuta da `P1`.

Nel momento in cui è presente un ciclo del genere possiamo essere **sicuri** che tra questi processi è **possibile** che avvenga una situazione di deadlock.

Invece possiamo considerare che **si verifica** il deadlock se i processi partono da una condizione iniziale in cui hanno già in possesso le rispettive risorse e facciano una richiesta per l'altra, quando nessuno dei due processi ha terminato.

Ovviamente nel caso in cui le risorse avessero più istanze non ci sarebbe un problema, perché i due processi accederebbero a istanze diverse e quindi non si violerebbe la **mutua esclusione**.

<p align='center'><img src='images/grafo_nodeadolock.png' width='300' ></p>

→ Questo **non** significa che avere più istanze **risolva** il problema, perché basta che si aggiungano altri processi al grafo con una particolare configurazione che la situazione di deadlock potrebbe accadere.

OSSERVAZIONI:

- Un ciclo è una condizione sufficiente per la possibile condizione di deadlock.
  
  → Se il grafo **non contiene cicli** ⟹ non si verificano situazioni di stallo

- Se il grafo **contiene un ciclo** ⟹ si potrebbe verificare una situazione di stallo, la cui possibilità diminuisce con il numero di istanze per ogni risorsa.
  
  Quindi la possibilità che ci sia un deadlock esiste ma non è detto che si verifica, perché tutto dipende anche dalla velocità relativa di esecuzione di ogni processo.

Un caso che possiamo considerare UTOPICO è quando ogni risorsa ha tante istanze quanti siano i processi che potenzialmente possano richiederla → impossibile proprio perché non sappiamo il numero di processi che potenzialmente è troppo elevato e le risorse sono limitate.

Linux cosa fa? Quando rileva un deadlock tenta di **eliminare** (a posteriori) questa condizione andando a terminare uno dei processi scatenanti, la cui scelta dipende da delle metriche.

Quindi accetta che questa situazione può verificarsi e nel momento in cui è tale risolve il problema mediante una sua politica di gestione.

## Metodi per la gestione dei deadlock

Esistono diversi approcci per gestire la situazione di deadlock che consistono principalmente in prevenirli (a priori) o rilevarli (a posteriori).

1) Prevenzione dei deadlock (**PREVENTION**):
   
   rendere **impossibile** il verificarsi delle **condizioni di deadlock**, ma al costo di un basso utilizzo delle risorse.

   Questo approccio tenta di annullare le condizioni che possono causare un deadlock → evitano che si creino dei cicli nel grafo di assegnazione delle risorse.

   Ma nel fare questo **limitano l'utilizzo delle risorse non sfruttandole a pieno**, quindi rallentando il sistema.
2) Evitare i deadlock (**AVOIDANCE**):
   
   le condizioni per il deadlock sono consentite, quindi si ammette l'esistenza delle condizioni tali per cui esso può avvenire, me il sistema **evita di entrare** in uno stato di deadlock.

   Evita la condizione **analizzando ogni richiesta** di risorse prima di concederla, e **accettandola solo** se non porta il sistema in uno "**stato non sicuro**".
3) Rilevazione del deadlock (**DETECTION**):
   
   Si permette al sistema di entrare in uno stato di deadlock, per poi risolvere il problema (**ripristino il sistema**).

   Quindi si ammette che ci possa esser la condizione di deadlock, non si fa nulla per evitarla, ma nel momento in cui questa si verifica il sistema tenta di tornare in uno stato sicuro.

   Ovvero si gestisce il deadlock solo dopo che questo si verifica.

La maggioranza dei sistemi operativi general-porpose, inclusi UNIX e Windows, **non dispone di una soluzione generale ed efficiente** al problema del deadlock.

Poiché tutte le politiche di gestione elencate hanno grandi problemi per cui non possono essere adottati.

NOTA: se creo due processi e faccio in modo che questi vadino in deadlock, il sistema operativo non fa nulla. Sarà un problema demandato al programmatore offrire una soluzione alla specifica situazione.

## Condizioni per il deadlock

Le condizioni **necessarie** sono:

- **Mutua esclusione** → un processo per alla volta può usare la risorsa.
- **Impossibilità di prelazione** → una risorsa può esser rilasciata **solo volontariamente** dal processo che la possiede, al termine della sua esecuzione. 
  
  Quindi non esistono casi in cui a un processo venga prelazionata una risorsa a favore di un altro che la richiede.
- **Possesso e attesa** → un processo che possiede almeno una risorsa, **attende di acquisire ulteriori risorse** già possedute da altri processi.

Invece le condizioni **necessarie e sufficienti** sono (tutte quelle necessarie + attesa circolare):

- **Mutua esclusione**
- **Impossibilità di prelazione**
- **Possesso e attesa**
- **Attesa circolare** → che abbiamo visto essere una **condizione necessaria e sufficiente**.

Affinché ci sia attesa circolare devono necessariamente esser verificate tutte le condizioni necessarie. Quindi possiamo dire che se è presente un'attesa circolare allora automaticamente è presente una condizione di deadlock.

(Attesa circolare è una conseguenza di un ciclo nel grafo di assegnazione, ma non è detto che si verifichi)

Attesa circolare è la condizione che si verifica nel momento in cui

- esiste un insieme {P0, P1, P2, ..., Pn} di processi in attesa, tali che:
  
  - P0 è in attesa per una risorsa che è posseduta da P1
  - P1 è in attesa per una risorsa che è posseduta da P2
  - ...
  - Pn-1 è in attesa per una risorsa che è posseduta da Pn
  - Pn è in attesa per una risorsa che è posseduta da P0
  
  Ovvero quando nel grafo di assegnazione si crea un ciclo di attesa perché non ci sono abbastanza istanze di risorse disponibili affinché almeno uno di questi processi non ha bisogno di attendere.

<p align='center'><img src='images/attesa_circolare.png' width='600' ></p>

## Deadlock PREVENTION

Nella deadlock **prevention**, di evita il deadlock **invalidando** una delle quattro condizioni necessarie e sufficienti.

Svantaggi nell'utilizzo di questo tipo di gestione del deadlock:

1) mancato uso di risorse che sono disponibili;
2) esecuzione rallentata dei processi. → la politica di gestione costringe a processi ad attendere anche quando non è necessario
3) non è possibile alcun tipo di cooperazione tra processi

### Mutua esclusione

La mutua esclusione è imposta dalle caratteristiche della risorsa e spesso non è rimovibile a meno che non si serializzi l'esecuzione dei processi, quindi non si necessita di un mutex.

In questo caso però peggiorano molto le performance perché non si sfrutta più la concorrenza tra i processi.

- Può esse rilassata in alcuni casi di risorse condivisibili
  
  come, ad esempio, le risorse read-only.

- Comporta costi maggiori → il sistema deve garantire che una risorsa critica (che potrebbe generare inconsistenze) non sia accessibile da più processi che ne fanno richiesta.
  
  Quindi che la risorsa è posseduta da un solo processo per volta.

### Possesso e attesa

Eliminando questa condizione si forza un processo a **richiedere una risorsa solo quando non ne possiede altre** (es. all'avvio richiede tutte le risorse necessarie alla sue esecuzione).

In questo caso si deve implementare una sorta di dichiarazione per ogni processo delle risorse che utilizza. 

Tale dichiarazione deve contenere tutte le risorse necessarie che verranno bloccate per tutta l'esecuzione del processo. Quindi si può capire che è molto inefficiente come soluzione perché le risorse vengono bloccate per tutta l'esecuzione anche se il processo le utilizza in una piccola parte.

→ Approccio soggetto a **starvation**, perché potrebbe esistere un processo che è sempre in esecuzione e utilizza una risorsa che non potrà mai essere utilizzata da altri processi.

<p align='center'><img src='images/rimozione_possesso_e_attesa.png' width='600' ></p>

- In questo caso se P1 non termina mai → P2 non potrà mai terminare la propria esecuzione.

### Impossibilità di prelazione

Rilassando il vincolo di impossibilità di prelazione se un processo già possiede alcune risorse, e ne richiede un'altra che non gli può esser allocata immediatamente, allora **rilascia tutte le risorse possedute**.

Quindi non si mette in attesa per la singola risorsa che richiede mantenendo il possesso di quelle già allocate, ma libera tutte le risorse e si mette in attesa.

Tale processo quindi non si metterà in attesa per la sola risorsa in più richiesta ma anche per tutte le altre che possedeva e che ha rilasciato.

→ il processo verrà eseguito nuovamente solo quando può riottenere il possesso sia delle **vecchie che delle nuove risorse**.

<!-- @todo inserisci un immagine o una gif esemplificativa -->

### Attesa circolare

Si stabilisce a priori un **ordinamento totale** tra tutte le risorse.

E si richiede che ogni processo richieda le risorse seguendo l'ordine prestabilito.

Quindi se un processo ha bisogno di utilizzare un certo numero di risorse deve richiedere l'accesso a queste nell'ordine prestabilito nonostante tale ordine non sia quello delle operazioni che effettua su queste.

Nell'esempio successivo P2 chiede l'accesso prima a "disco 1" poi a "disco 2" nonostante operi inizialmente solo su "disco 2".

→ provoca una perdita delle performance perché un processo potrebbe possedere una risorsa per un tempo che è molto superiore rispetto al tempo effettivo nel quale opera su tale risorsa.

ESEMPIO: 

Ordine imposto:

1) disco 1
2) disco 2

<div style="display:flex; gap:20px;">

  <!-- Colonna P1 -->
<div style="border:2px solid #b7c0e3; background:#eef1ff; padding:15px; width:45%; border-radius:6px; color:#2b2b2b;">
  <b>P₁</b><br><br>
  …<br><br>
  <div style="border:2px dashed red; padding:6px; border-radius:4px; color:#2b2b2b;">
  wait (<span style="color:red;">mutex1</span>)<br>
  &lt;inizio uso disco 1&gt;
  </div>
  …<br><br>
  wait (<span style="color:green;">mutex2</span>)

  &lt;inizio uso disco 2&gt;<br>
  …
</div>

  <!-- Colonna P2 -->
<div style="border:2px solid #d0d0d0; background:#f5f5f5; padding:15px; width:45%; border-radius:6px; color:#2b2b2b;">
  <b>P₂</b><br><br>
  …<br><br>
  <div style="border:2px dashed red; padding:6px; border-radius:4px; color:#2b2b2b;">
  wait (<span style="color:red;">mutex1</span>)
  <br>
  wait (<span style="color:green;">mutex2</span>)<br>
  </div>
  &lt;inizio uso disco 2&gt;<br>
  
  …<br>

  &lt;inizio uso disco 1&gt;<br>
  …<br><br>
</div>

</div>

- In questo caso, supponendo che P2 faccia per **primo** la prima richiesta delle risorse:
  
  - P1 è **impossibilitato a usare "disco 1"** anche se P2 sta usando "disco 2".
  - Si è imposto un ordine di acquisizione delle risorse (a discapito dell'efficienza).

Questo tipo di approccio per la gestione PREVENTION non permette l'implementazione di una cooperazione (come anche gli altri approcci) tra processi.

→ implementando il problema produttori consumatori otteniamo che i produttori producono sempre fino a che non terminano. Solo dopo la terminazione dei produttori i consumatori potranno accedere ai dati prodotti.
→ comportamento non richiesto per l'implementazione.

## Deadlock AVOIDANCE

Nella gestione AVOIDANCE il sistema decide **a tempo di esecuzione** se una richiesta di una risorsa può portare a un deadlock (**prevenzione dinamica**).

- **nessun vincolo a priori** delle risorse
- se lo stato attuale delle risorse è **rischioso**, un algoritmo **rifiuta la richiesta** di allocazione

Quindi si accetta la possibilità di incorrere in un deadlock, non eliminando le condizione necessarie, ma si cerca di evitarlo valutando lo stato in cui si trova in sistema ogni volta che viene effettuata una richiesta (a *run-time*).

Quindi istante per istante, possedendo **la storia precedente del grafo delle assegnazioni delle risorse**, un algoritmo valuta se **successivamente** a una certa richiesta da parte di un processo porta l'applicazione a un deadlock.

Quindi l'algoritmo deve essere in grado di fare una **sorta di predizione sull'andamento dell'esecuzione** dei processi sugli istanti successivi a una qualsiasi richiesta per una risorsa.

Quindi possiamo considerarla come una prevenzione, che **non è più statica** come per la gestione PREVENTION, ma **dinamica**.

Anche questa come soluzione al deadlock è molto complicata perché richiede diverse assunzioni: come quella di riuscire a prevedere le eventuali richieste di un processo se questo non le dichiara a priori.

<p align='center'><img src='images/avoidance_esempio.png' width='500' ></p>

La memoria totale è `200 kb`.

- P2 non avrà accesso alla risorsa nel momento in cui la richiede perché il processo P1 è già in esecuzione e possiede già una istanza della stessa risorsa.

  Però il motivo per cui viene rifiutata la richiesta di P2 è perché guardando la storia di P1 si nota che affinché questo possa terminare dovrà ottenere un'altra istanza della risorsa.

  Nel momento in cui richiederà un'altra istanza della risorsa non ci sarà più spazio per l'istanza richiesta da P2

  - Caso in cui avviene il deadlock →  P2 riceva la risorsa (supponendo che successivamente ne richieda un'altra da `80 kb`).

    → Quindi la memoria in possesso sarà `80 + 70 = 150kb`.

    → Il processo P1 si sospende perché ne richiede `70kb` e allo stesso modo si sospende P2 perché ne richiede, come detto, `80kb`.

    → Entrambi i processi sono sospesi in attesa che l'altro rilasci la risorsa: deadlock.
  
  Nell'esempio un algoritmo ha valutato questa situazione conoscendo la storia di allocazione dei processi.

Quindi è necessario supporre che per ogni processo si deve dichiarare la **storia di allocazione**, altrimenti non di possono fare previsioni sull'andamento dell'esecuzione.

Questo produce un **overhead** elevato sullo sheduler e in generale sul kernel. Ma da una soluzione per non entrare nel deadlock.

Linux implementa ciò? No perché non si conosce a priori la storia di allocazione di ogni processo → è impossibile prevedere un deadlock in questo modo.

<div style="border:3px solid #202092ff; background:#eef1ff; padding:12px; border-radius:6px; color:#555555">
  <b style="color:#d40000;">**Presupposto**</b>: queste tecniche richiedono di
  <span style="color:#d40000;">conoscere in anticipo</span>
  tutte le richieste che un processo può fare nell’arco della sua esecuzione.
</div>

Questa è un assunzione molto pesante, un caso più semplice di utilizzo è quello che: ogni processo **dichiara** il **numero massimo** di istanze di risorse di cui può avere bisogno.

→ Tali istanze però potrebbero non esser utilizzate subito e quindi tolte ad altri processi che potrebbero sfruttarle immediatamente.

### Approcci

Abbiamo due diversi approcci per questo tipo di gestione:

1) **Process initiation Denial**: all'avvio di un nuovo processo (rifiuto l'esecuzione del processo)
   
   Non si avvia un processo se le sue richieste potrebbero portare ad un deadlock
2) **Resource Allocation Denial**: al momento di una richiesta di allocare una risorsa. (il processo esegue ma possono essere vietate le richieste nonostante la disponibilità corrente è valida)
   
   Si consente l'avvio, ma le richieste di allocazione possono essere rifiutate se possono portare a deadlock.

---

Entrambi questi approcci si basano sulla costruzione di diverse strutture algebriche:

sia *n* = numero di processi, 

e *m* = numero di tipi di risorse

- **Resource** = *R* = (R1, ..., Rm)
  
  Risorse totali nel sistema. Ri p il numero di istanze presenti nel sistema per la risorsa i-esima.
- **Available** = *V* = (V1, ..., Vm)
  
  Numero di istanze per ogni risorsa non allocate ad alcun processo. Vi rappresenta il # di istanze della risorsa Ri non ancora allocate.
- **Claim** = *C* = matrice *n x m*
  
  Cij = richiesta del processo Pi per la risorsa Rj
- **Allocation** = *A* = matrice *n x m*
  
  Aij = allocazione corrente al processo Pi della risorsa Rj

---

### Process Initiation Denial

- La matrice *C* (di richiesta) indica **il numero massimo di richieste** per ogni processo (righe), di una certa risorsa (colonne).
- Deve essere fornita prima dell'avvio dei processi
  
  Quindi questa matrice verrà aggiornata e ricalcolata ogni volta che un processo termina o inizia la sua esecuzione.

<p align='center'><img src='images/c.png' width='300' ></p>

Un processo Pn+1 viene eseguito solo se: 

<a id="legge_numero_richieste_massimo"></a>
<p align='center'><img src='images/numero_richieste_massimo.png' width='300' ></p>

Cioè un processo viene eseguito se il **numero massimo di richieste di tutti i processi** (sommatoria) **più quelle del nuovo processo** (Cij) per la risorse j-esima è minore del numero di istanze della richiesta Rj.

Questo deve esser calcolato per ogni tipo di risorsa → per ogni colonna di *C*.

ESEMPIO:

<p align='center'><img src='images/esempio_process_initiation_denial.png' width='600' ></p>

In questo esempio P1 non viene eseguito fin tanto che P2 non termina la sua esecuzione (almeno).

Il motivo per cui non viene eseguito è proprio il [vincolo](#legge_numero_richieste_massimo) imposto da process initiation denial:

- nel momento in cui P1 tenta di esser eseguito.
  
  Per MB di memoria:\
  100 (R1) >= 70 (C11) + 70 (C21) + 0 (C31) = 140 ⟹  non verificato

  Per la porta seriale:\
  1 (R2) >= 1 (C12) + 1 (C22) + 0 (C32) = 2 ⟹  non verificato

### Resource Allocation Denial

Tale approccio viene chiamato anche **algoritmo del banchiere**

→ viene eseguito ad ogni tentativo di allocazione;

- se l'allocazione può portare ad uno stato "non-sicuro" viene rifiutata.

L'algoritmo fa in modo che lo stato del sistema (risorse e processi) **non sia mai uno stato non sicuro**

<p align='center'><img src='images/stato_sistema.png' width='300' ></p>

La **strategia** consiste nel trovare una sequenza di esecuzione *safe*.\
La sicurezza di uno stato dipende dalle risorse disponibili, e dalle richieste di tutti i processi nel sistema.

<p align='center'><img src='images/esempio_stato_non_safe.png' width='550' ></p>

Quindi l'obiettivo è fare in modo di determinare una sequenza di esecuzione che non faccia mai entrare lo stato del sistema nella porzione del piano *unsafe*.

ESEMPIO:

<p align='center'><img src='images/esempio_unsafe.png' width='500' ></p>

In questo esempio si può vedere l'avanzamento dell'esecuzione di P1 e P2.

<!-- @fix continua.... pag43 ppt SO15 -->
