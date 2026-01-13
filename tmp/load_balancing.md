# load balancing

In Linux qualunque processore può eseguire qualunque task.

Quindi un task può esser assegnato a qualsiasi processore.

Sfrutta il dynamic load balancing per associare i task ai vari processori in una architettura SMP.

L'algoritmo che determina il trasferimento di un task dalla coda di un processore a quella di un altro si basa sul concetto di CPU affinity.

CPU affinity: lega l'esecuzione di un flusso di codice sequenziale al processore sul quale l'esecuzione è più veloce/efficiente. Ovvero saranno presenti meno cache miss nel caso in cui il task esegue su tale processore.

Se un task esegue su un processore, è preferibile che questo sia riposizionato nuovamente nella coda dello stesso processore per sfruttare il principio di località (cache hot).

Per ottenere un throughput elevato in un sistema SMP, è necessario che il numero di task in esecuzione sia equilibrato tra i vari processori.

Quindi bisogna fare un compromesso tra aumentare il throughput oppure rispettare la CPU affinity quando si tenta di trasferire un task da un processore ad un altro.

Il load balancing è attivato da `shedule()` periodicamente, o quando una runqueue è vuota.

Vengono **estratti dalla lista i task che non stanno eseguendo e non sono cache-hot**.

L'algoritmo termina quando la runqueue con il maggior numero di task **non eccede del $25\%$** le altre runqueue.

Tale percentuale è modificabile all'interno dello pseudo file system `/proc`.

