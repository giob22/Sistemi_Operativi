<script src="../javascript.js"></script>

# ABI

La **Application Binary Interface** (**ABI**) è un insieme di convenzioni tra applicazioni utente e SO.

Il kernel ne garantisce la stabilità ad ogni versione.

Stabilisce il modo con cui avviene il passaggio dei parametri all'atto della chiamata del `syscall_handler`. Infatti i parametri non vengono passati tramite lo stack lato user o direttamente lo stack lato kernel, perché:

1) il kernel non può accedere allo spazio utente direttamente, per questioni di sicurezza.
2) il processo user non può scrivere direttamente nello stack kernel, il suo accesso è negato se non si trova in kernel mode

Quindi il passaggio avviene tramite i registri del processore.

- Velocissimi: non richiedono accessi alla memoria
- Isolati: quando avviene il salto nel kernel, i valori sono già lì, pronti pre essere letti senza rischi di modifiche esterne o errori di memoria

I registri sono limitati, ma questo non porta grosse problematiche perché è molto difficile che una chiamata di sistema possa richiedere molti parametri di ingresso da sforare la quantità di registri general-porpose disponibili.

Il motivo di ciò è principalmente dovuto al testing delle syscall, tanti parametri significa grande superficie di attacco. Quindi nel caso di tanti parametri i casi di testing crescono esponenzialmente.

Nell'ipotesi di molti parametri di ingresso si potrebbe utilizzare come parametro un puntatore ad una struttura che deve essere letta dal kernel mediante la funzione `copy_from_user`, altrimenti potrebbero generarsi risultati non desiderati.

Ad esempio se non si facesse una copia dei dati e successivamente un controllo su questi, ma si facesse unicamente il controllo:\
il processo utente chiedere di leggere un file normale, a cui ha accesso, il kernel controlla che effettivamente ha il permesso di farlo accedendo tramite il puntatore alla memoria; se prima che il kernel esegua la procedura per la lettura un thread malevolo acceda all'area di memoria in cui è contenuta la struttura, in user space, potrebbe modifica il file da leggere inserendo invece un file non ordinario ma contenente informazioni critiche.
