# moduli

A differenza dei processi utente, un bug nella gestione della memoria nel kerel rende instabile l'intero sistema.

Quindi un mal gestione della memoria all'interno di un modulo potrebbe essere un problema grave che potrebbe causare anche il blocco dell'intero sistema, o kernel panic.

Infatti a valle di un eccezione occorsa in kernel mode viene prodotto un messaggio di oops e il kernel tenta di risolvere l'eccezione. Per ripristinare il sistema tenta di terminare il processo utente attraverso il quale stava eseguendo. Ma se l'eccezione é grave e ha corrotto strutture dati fondamentali, oppure non é presenta alcun processo utente, allora il kernel lancia la funzione `panic()` e il sistema si blocca.

Il kernel non puó utilizzare le funzioni di libreria C

Si utilizzano invece dei sostituti

- logging: `printk()`
- allocazione memoria: `kmem_cache_alloc()`, `kmalloc()`
- sincronizzazione: `spin_lock()`, `unlock()`, `wake_up_interruptible()`
- ...

Avvertenze per la programmazione di sistema:

- l'aritmetica floating point non é sempre di facile utilizzo, per via della gestione delle eccezioni
- gli stack del kernel sono molto limitati, 1 o 2 pagine. Ció significa che é difficile che possiamo utilizzare delle funzione ricorsive di cui non conosciamo a priori la profonditá.
- Occorre gestire la sincronizzazione con altre parti del kernel, che accedono a risorse, come strutture dati, comuni.

