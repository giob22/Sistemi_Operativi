# Porzione dedicata la kernel nello spazio di indirizzamento virtule

In ogni spazio di indirizzamento virtuale é presente una porzione di indirizzi virtuali riservati al kernel

Tali indirizzi virtuali sono utilizzati per mappare lo stack, lo heap, l'area testo del kernel

In modo tale che quando il processo esegue una syscall non avviene un cambio di contesto. Il kernel utilizzerá lo stesso spazio di indirzzamento.

In questo modo si evita che l'MMU debba cambiare page table e che la cache TLB sia invalidata completamente.

Quindi il kernel utlizza lo stesso spazio virtuale del processo corrente per eseguire codice kernel. Tale porzione mappa parte dell'immagine del kernel.

in questa porzione é anche contenuto il mapping per le strutture dati necessarie al kernel per gestire i processi e le varie risorse.

Questo approccio introduce peró dei problemi di sicurezza. Infatti questa vulnerabilitá é sfruttata da Meltdown e Spctre per accedere a dati sensibili mantenuti nel kernel.

Il modo con cui vengono tradotti gli indirizzi virtuali dedicati al kernel non avviene tramite la page table utilizzata per lo user space.

Tale traduzione avviene tramite l'impiego dell MAster kernel page table. Condivisa tra tutti i processi.

L'associazione tra indirizzo virtuale della porzione dedicata al kernel é **lineare**.

Essendo una associazione statica é una fonte di vulnerabilitá perché conoscendo l'offset é possibile ricavare l'indirizzo fisico in cui é mappato il kernel.

L'utilizzo della porzione di spazio di indirizzamento dedicata al kernel per ogni processo é stato causa di problemi di sicurezza per via delle moderne CPU che fanno uso della esecuzione speculativa.

Ovvero il processore tenta di indovinare le prossime istruzioni da eseguire e le esegue per portarsi avanti nell'esecuzione.

Quando la predizione va a buon fine il processore si trova quelle istruzioni giá eseguite, quindi guadagna tempo.

invece se la predizione che fa non é quella corrette fa una recovery delle istruzioni che ha eseguito prima di continuare l'esecuzione.

Il problema sta nel fatto che se un utente non ha i permessi per accedere a contenuti sensibili attraverso delle operazioni di lettura, il processore potrebbe comunque leggerli nel caso l'utente ottenga i permessi per via del meccanismo della esecuzione speculativa.

Quindi se l'utente non ha i permessi fa una operazione di recovery, quindi l'operazione di lettura non va a buon fine.

Fin qui tutto bene, il problema sta nelle cache: essendo che il processore per eseguire le istruzioni future secondo il meccanismo dell'escuzione speculativa avrá letto delle informazioni sensibili che saranno copiate nelle cache.

Quindi il contenuto di queste adesso si trova nelle cache e tramite delle tecniche potrebbero esser comunque letti.



