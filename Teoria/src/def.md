<script src="../javascript.js"></script>

# Definizioni utili

- ***Processo***:
  
  Unità base di esecuzione del SO, descrive **l'attività dell'elaboratore relative alla specifica esecuzione di un codice**.

  è un'entità dinamica che consiste in: programma da eseguire + stato di esecuzione.

  Possiamo avere più processi relativi all'esecuzione di più istanze dello stesso programma.

- ***Programma***
  
  Sequenza statica di esecuzione, corrisponde alla codifica in un linguaggio di programmazione comprensibile all'elaboratore, in modo che questo possa essere eseguito.

- ***Immagine di un processo***

  L'immagine di un processo consiste nell'insieme del codice, area dati globale, stack, heap e il descrittore. Ovvero tutte quelle componenti necessarie all'esecuzione in un contesto di sistema operativo multiprogrammato.

  - Stack
  - Heap
  - Area dati globali
  - Codice → rientrante
  - PCB
  - Stack del kernel → utilizzato dal kernel quando il processo esegue in kernel mode

- ***Spazio di indirizzamento***
  
  L'immagine del processo e le risorse da esso possedute costituiscono il suo spazio di indirizzamento.


- ***Thread***
  
  Flusso di controllo sequenziale di un processo eseguito in maniera concorrente a questo.

  A differenza di un processo non possiede delle risorse private alla sua esecuzione ma le condivide con altri thread dello stesso processo e il processo stesso.

  Quello che appartiene ad un thread - processo leggero - sono:

  - insieme di registri
  - stack
  
  Tutte le altre aree di memoria fanno parte dello spazio di indirizzamento del processo, quindi condivise.



