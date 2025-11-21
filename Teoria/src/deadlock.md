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

Invece le condizioni **sufficienti** sono (tutte quelle necessarie + attesa circolare):

- **Mutua esclusione**
- **Impossibilità di prelazione**
- **Possesso e attesa**
- **Attesa circolare** → che abbiamo visto essere una **condizione necessaria e sufficiente**.

Affinché ci sia attesa circolare devono necessariamente esser verificate tutte le condizioni necessarie. Quindi possiamo dire che se è presente un'attesa circolare allora automaticamente è presente una condizione di deadlock.

(Attesa circolare è una conseguenza di un ciclo nel grafo di assegnazione, ma non è detto che si verifichi)
