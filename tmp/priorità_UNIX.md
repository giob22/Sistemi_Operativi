# il calcolo della priorità dinamica di UNIX

In unix la priorità di un processo i basa sul concetto di feedback, ovvero si considera il tempo che il processo ha eseguito sul processore precedentemente.

Tale tempo è diviso per due e sommato al nice value e ad un fattore di base che potrebbe dipendere dalla natura del processo.

Il nice value è un fattore di aggiustamento che l'utente può modificare assegnandogli un valore tra $[-20; 19]$.

Lo scheduling viene attivato ogni qual volta entra nella coda dei processi pronti un processo, valutando quindi se tale processo ha priorità maggiore rispetto al processo che è in quel momento in esecuzione.

Se si allora avviene una cambio di contesto a favore del processo con maggiore priorità.

