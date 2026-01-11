# caricamento a domanda: obiettivo

L'obiettivo del caricamento a domanda é di usare in modo efficiente la memoria centrale disponibili.

Quindi é quello di far in modo che nella memoria centrale siano contenute unicamente le pagine necessarie all'esecuzione di un processo e non l'intera immagine del processo.

Il meccanismo della paginazione a domanda funziona bene per il principio di localitá, infatti verranno caricate in memoria unicamente le pagine che sono accedute frequentemente dal processo durante la sua esecuzione.

Infatti durante la sua esecuzione il processo non accederá mai all'intero spazio di indirizzamento, ma accede ad un sottoinsieme di pagine dello spazio di indirizzamento

Questo approccio mi permette anche di non tener conto dell'effettiva grandezza della memoria virtuale che potrebbe anche essere molto piú grande della memoria fisica.

Questo é possibile perché una pagina virtuale viene mappata in memoria fisica unicamente quando il processo ne ha bisogno

Se la memoria fisica si satura oppure supera una certa quantitá di pagine allocate per un processo allora il kernel esegue un algoritmo di sostituzione di pagina. Questo algoritmo peró deve seguire una metrica che eviti di sostituire e quindi invalidare pagine che fanno parte del working set di un processo.

Il working set di un processo é l'insieme di pagine che sono piú frequentemente utilizzate dal processo durante la sua esecuzione. Quindi andando a sostituirle non sfruttiamo il principio di localitá e come consequenza otteniamo un page foult.

Tanti page foult possono rallentare l'esecuzione dei processi perché questi devono attendere che la pagina necessaria per la loro esecuzione sia allocata nella memoria centrale. Questa operazione richiede l'uso del disco che ha dinamiche molto piú lente rispetto le dinamiche di un processore.

Nel caso in cui il grado di multiprogrammazione sia elevato al punto che la memoria principale non é in grado di contenere per intero le pagine del working set di ogni processo potrebbe manifestarsi una attivitá di paginazione degenere.

Attvitá di paginazione degenere = i processi si sottraggono a vicenda pagina del working set in memoria centrale.

Ovvero l'algoritmo di sostituzione di pagina inevitabilmente va a scegliere come pagina vittima una pagina del working set di un processo, quindi nel momento in cui il processo della pagina vittima ritorna ad eseguire, per il principio di localitá tenterá di accedere a quella pagina che non é piú mappata in memoria centrale, page foult.

Quindi in questa situazione si generano moltissimi page foult tanto che il processore spende piú tempo a gestirli che ad eseguire codice utile all'esecuzione di un processo. Inoltre questa condizione potrebbe arrivare ad essere patologica perché lo scheduler a lungo termine vedendo un basso utilizzo del processore permette l'ammissione di sempre piú processi che richiedono sempre pagine per il proprio working set.

Quindi si innesca un ciclo vizioso che porta ad una attivitá di paginazione degenere patologica.

La soluzione che si deve adottare in questi casi é di limitare il grado di multiprogrammazione, facendo in modo che la dimensione dei working set dei processi attivi nel sistema non superi mai la dimensione della memoria centrale.

WWS < M

Ovvero per evitare il thrashing é necessario che per ogni processo siano allocate in memoria centrale almeno un set di pagine corrispondente al proprio working set.

Dove WWS = working set size 
M = dimensione della memoria centrale

Quindi in questo caso deve operare lo scheduler a medio termine che dovrá eseguire operazioni di swapping per limitare temporalmente la quantitá di processi attivi. 

Oppure si potrebbe terminare l'esecuzione di alcuni processi secondo sempre delle metriche per la scelta del processo da eliminare.

