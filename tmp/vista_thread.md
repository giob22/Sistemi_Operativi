# thread in linux

Più thread che operano che fanno parte dello stesso spazio di indirizzamento vengono visti dal livello utente come un unico processo, mentre a livello kernel si distingue ogni flusso di esecuzione.

Infatti eseguendo `getpid()` a livello utente otteniamo il `TGID` del thread e non il `PID`. Quindi se eseguissi per ogni thread la funzione `getpid()` otterrei lo stesso risultato perché fanno parte dello stessa spazio di indirizzamento.

A livello kernel inveve si identifica come task ogni flusso di esecuzione, quindi sia processi con un unico flusso di esecuzione sia thread che corrispondono a più flussi di esecuzione nello stesso processo.

