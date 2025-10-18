# Earliest Eligible Virtual Deadline First Scheduler (EEVDF)

Rispetto a `CFS`, `EEVDF` rimuove alcune euristiche e introduce il **concetto di virtual deadline**, per dare priorità ai task in debito di CPU o con timeslice più stringenti.



In linux abbiamo il concetto di control block, fondamentale quanto introdurremo la funzione legata alla virutalizzazione leggera. Qando la introdurremo parleremo di tecnologie di virtualizzazioni classiche -> emulare un processore fisico. Esiste un software chimato Hypervisore che ci permette di avere una achitettura sottostante (host) di un certo tipo e un'architettura e l'architettura da emulare detta guest. potremmo eseguire codice arm su x86.

Tutta la virtualizzazione basata sul sistema operativo utilzza il kernel stesso, che più che un virtualizzatore è un divisore di risorse. Questo concetto di divisione di risorse già lo abbiamo -> processo tramite il sistema operativo è la virtualizzazione di un processore.

L'idea di base è che il kernel già ci fa questa virtualizzazione.


Perfetto 👍 quindi ti riferisci alla **slide 16**, quella **prima della 17**, intitolata:

> **“…le problematiche”**

Vediamola insieme:

---

### 🧾 Testo della slide 16

> **…le problematiche**
>
> * Comunicazione tra processi
> * Risorse condivise
> * Sincronizzazione tra processi
> * Assegnazione del tempo del processore

---

Questa slide elenca le **principali difficoltà** che emergono quando più processi o thread vengono eseguiti **in concorrenza**.
Vediamole una per una 👇

---

## 1️⃣ **Comunicazione tra processi**

Quando i processi lavorano in modo concorrente, spesso **devono scambiarsi informazioni** (dati, segnali, risultati, messaggi…).
Tuttavia, ogni processo ha **il proprio spazio di memoria**, quindi **non può accedere direttamente** alle variabili di un altro.

### 💡 Problema:

Come far comunicare i processi in modo **sicuro e coordinato**?

### ⚙️ Soluzioni tipiche:

* **Memoria condivisa** → variabili globali protette da meccanismi di sincronizzazione.
* **Scambio di messaggi** → funzioni `send()` e `receive()` tra processi.

---

## 2️⃣ **Risorse condivise**

Le **risorse** (file, variabili, stampanti, CPU, memoria, ecc.) possono essere **utilizzate da più processi**.
Se due processi accedono contemporaneamente alla **stessa risorsa**, si possono verificare errori o incoerenze.

### 💡 Problema:

Come evitare che due processi usino contemporaneamente una risorsa che non lo permette?

### ⚙️ Soluzione:

Serve la **mutua esclusione**:

> solo un processo per volta può accedere alla risorsa.

Si ottiene con strumenti come:

* **semafori**,
* **mutex (mutual exclusion lock)**,
* **monitor**,
* **regioni critiche**.

---

## 3️⃣ **Sincronizzazione tra processi**

Quando due processi collaborano, l’ordine in cui si eseguono **le loro operazioni deve essere coordinato**.

### 💡 Problema:

Se un processo dipende dai dati prodotti da un altro, deve **aspettare** che quel dato sia pronto — non può andare avanti da solo.

Esempio:

* Il processo A legge da un buffer.
* Il processo B scrive nel buffer.
  → A deve aspettare finché B non ha finito di scrivere.

### ⚙️ Soluzione:

* **Primitive di sincronizzazione** (`wait`, `signal`, `join`, `barrier`, ecc.)
* **Monitor**, **condition variables**, ecc.

---

## 4️⃣ **Assegnazione del tempo del processore**

In un sistema con **più processi attivi**, tutti vogliono usare la CPU.
Ma se c’è **una sola CPU**, bisogna decidere **chi esegue e per quanto tempo**.

Questo è compito dello **scheduler**, che applica una **politica di scheduling** (es. round robin, priorità, CFS, ecc.).

### 💡 Problema:

Come distribuire il tempo CPU in modo **equo ed efficiente**?

### ⚙️ Soluzione:

* Meccanismi di **time-sharing** (divisione del tempo in *quantum*),
* **Scheduler** che assegna la CPU in base a priorità, tempi di attesa, ecc.

---

## 🧩 In sintesi

| Problematiche                  | Descrizione                             | Soluzioni                         |
| ------------------------------ | --------------------------------------- | --------------------------------- |
| **Comunicazione**              | Scambio di informazioni tra processi    | Memoria condivisa, messaggi       |
| **Risorse condivise**          | Accesso simultaneo a risorse comuni     | Mutua esclusione (lock, semafori) |
| **Sincronizzazione**           | Coordinamento dell’ordine di esecuzione | Wait/signal, join, barrier        |
| **Assegnazione del tempo CPU** | Divisione equa della CPU tra processi   | Scheduler, time-sharing           |

---

👉 In poche parole, la **slide 16** introduce i **quattro grandi problemi** della programmazione concorrente:

* Come si **parlano** i processi,
* Come si **condividono** le risorse,
* Come si **coordinano**,
* E come si **distribuisce** il tempo CPU.

---

Vuoi che ti spieghi poi la **slide 27**, dove questi problemi vengono ripresi nel contesto della **sincronizzazione** (lock, semafori, monitor, ecc.) per vedere come si risolvono?





