# unix

unix é un sistema operativo multiprogrammato a divisione di tempo

Utilizza processi pesanti con codice rientrante.

- spazio di indirizzamento privato

la funzione di scheduiling viene eseguita, `schedule()`:

- allo scadere di un time slice
- attivazione di un processo a maggiore prioritá
- termina una syscall, quindi il processo passa da preempted a running in user mode.

oltre alla funzione di schedulazione esegue altri servizi di rutine:

- load balancing
- aggiornamento delle prioritá

l'obiettivo principale é quello di privilegiare i processi interattivi.

Utilizza come algoritmo di scheduling:

- scheduling feedback multilevel queue
- ogni processo ha una prioritá
- ogni coda associata ad un livello di prioritá é gestita con una politica Roud Robin
- preemptive, ogni prelazione di un processo in esecuzione diminuisce di un livello la sua prioritá, quindi entrerá nella coda dei processi pronti con prioritá minore.
  
  A questo proposito puó presentarsi una situazione di starvation per i processi CPU bound, che hanno un'alta probabilitá che saranno prelazionati.

  Quindi per evitare ció si potrebbero utilizzare tecniche che permettono di ridurre la starvation come Aging, oppure la possibilitá ad un processo che si trova nell'ultima coda di poter rientrare nella coda ad alta prioritá dopo una prelazione
- le prioritá sono dinamiche, valutate ogni quanto

genera un overhead elevato per via di numerosi context switch, il calcolo della prioritá per ogni processo, gestione dei processi nelle code, e il loro trasferimento in code di altri processori (load balancing).

## linux

Il kernel linux non fa distinzione tra processo e thread, entrambi vengono classificati come task.

Un thread é un task particolare che condivide strutture con altri task, ovvero condivide lo spazio di indirizzamento a meno di uno user e kernel stack e di un task struct che sono riservati al thread.

ogni task é associato ad un PID, tutti i task che eseguono nello stessso spazio di indirizzamento, ovvero che fanno parte di un processo, hanno lo stessa TGID, ovvero thread group ID.

Questo serve al kernel per capire quali task rappresentano flussi di esecuzione all'interno dello stesso processo.

Inoltre a livello utente i task che corrispondo a thread di un processo vengono visti come un unico processo, infatti se provassimo ad eseguire getpid su ogni thread otteniamo sempre lo stesso PID che in realtá sarebbbe il TGID.

Mentre a livello kernel ogni task é unitá eseguibile di schedulazione, quindi i thread non vengono visti come un unico processo. Il kernel li identifica mediante il PID che é univoco per ogni task.

Essendo che gestisce i thread a livello kernel significa che anche l'esecuzione del kernel puó esser strutturata in piú attivitá che vengono eseguite in parallelo o concorrentemente.

Il kernel linux permette di raggruppare piú task in un control group per:

- limitare l'uso di risorse di un gruppo
- prioritizzare l'accesso del gruppo di processi alle risorse, come avviene per le CPU nel caso di group scheduiling
- tracciabilitá del gruppo

CFS puó gestire i task in gruppi, che viene visto come una unica entitá di esecuzione schedulabile

Il time slice é assegnato all'intero gruppo, all'interno della time slice assegnata al gruppo saranno poi i task a spartirsi il tempo dipsonibile.

