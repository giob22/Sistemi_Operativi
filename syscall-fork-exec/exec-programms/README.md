# Exec()
---
L'unico modo in cui un programma può esser eseguito da UNIX è che il processo corrente invochi una chaimata di sistema appartente alla famiglia `exec()`. (questa chiamata di sistema può esser effettuata da qualsiasi processo tranne per _init_). Il nuovo programma viene eseguito nel contesto del processo precedente chiamante, ovvero il **PID** non varia. Tale chiamata di sistema viene anche detta _sostituzione di codice_.
<br><br>
Cosa accade al processo dopo la chiamata ad `exec()`:<br>
- Mantiene la stessa _process structure_;
- Ha codice, dati globali, stack e heap vuoti;
- Ha un riferimento ad una nuova _text structure_, contenente il nuovo programma da eseguire;
- Mantiene il **pid**
- Mantiene _user area_ (tranne il PC e informazioni legate al codice) e stack del kernel.
<br>
Quindi è un processo totalmente nuovo che filgio di un altro processo che esegue un programma differente. Proprio per questo motivo viene usata molto spesso la chiamata ad una `exec()` dopo una `fork()`.In Linux BSD viene implementata la `vfork()`, una system call che crea un processo senza copiare l'immagine del padre al figlio. Questo ovvimenete è molto vantaggio in termini di prestazioni perché ogni qual volta utilizziamo una `fork()` l'intera immagine del padre viene copiata al figlio, la quale sarà sovrascritta nel momento in cui verrà eseguita la `exec()`.
