# Parallelismo implicito

In questo tipo di parallelismo il codice che viene eseguito è scritto in modo sequenziale, ma sotto il cofano il sistema capisce quali parti sono idipendenti tra loro e le esegue insieme per risparmiare tempo.

Il parallelismo implicito avviene principalmente a due livelli:

- livello **hardware**
  
  Le moderne CPU non eseguono un'operazione per volta, utilizzano una tecnica che gli permette di valutare le prossime istruzioni che dovranno eseguire; questa tecnica è chiamata *Out-of-Order Execution*.

  La CPU quindi analizza le prossime istruzioni che dovrà, se queste sono indipendenti tra loro allora le lancia contemporaneamente su due unità di calcolo diverse.

  Oppure un'altra tecnica hardware è il *Pipelining*: mentre un'istruzione viene eseguita, la successiva viene già decodificata e quella dopo ancora viene prelevata dalla memoria. Quindi si sfrutta il ciclo del processore nell'eseguire singole istruzioni.

- livello **compilatore**
  
  Il compilatore può ottimizzare il programma ancora prima che questo venga eseguito dalla CPU.