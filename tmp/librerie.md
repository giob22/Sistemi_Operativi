# Librerie di moduli sofwtware

Le tecniche di sviluppo modulare consentono lo sviluppo su base professionale di librerie di moduli software.

Il produttore di una libreria distribuisce:

- i file di intestazione dei moduli che fanno parte della libreria;
- i moduli di libreria in formato oggetto (giá compilati), che l'utilizzatore deve collegare assieme ai propri moduli oggetto.

Tale scelta é tipicamente motivata da esigenze di tutela della prorietá, inoltre evita di dover ricompilare i moduli di libreria.

Ogni file oggetto che corrisponde ad una libreria contiene un set di funzioni che sono correlate tra loro.


## Tipi di libreria

- librerie statiche
  
  - dimensione dell'applicazione maggiore perché il modulo della libreria é collegato al file eseguibile staticamente
  - per queste librerie in caso di modifica é necessario ricompilare le applicazioni che ne fanno uso.
- librerie dinamiche (caricate a *run-time*)
  
  Manutenzione semplificata, perché non é necessario ricompilare tutte le applicazioni che fanno uso di questa libreria nel caso di modifica di alcuni moduli di questa

  Requisiti di spazio sul disco ridotti, perché ad essere memorizzato sul disco é unicamente l'applicazione senza contenere anche il codice eseguibile della libreria.

  Il codice della libreria viene linkato all'eseguibile dell'applicazione a tempo di caricamento.
- librerie dinamiche (collegate a *run-time*)
  
  Tutti i vantaggi dell'uso di librerie dinamica ma con l'aggiunta che queste possono esser condivise piú facilmente tra processi che ne fanno uso, e vengono caricate in memoria solamente quando é necessario utilizzare una delle procedure contenute all'interno dei moduli.

##  Librerie statiche

Una libreria statica é una collezione di moduli oggetto in un unico file, con estensione `.a`

Per creare una libreria statica si puó usare il comando `ar` di Unix, ad esempio

```shell
ar rcs lib[nome libreria].a file1.o file2.o
```

con:

- r: inserisce il file con rimpiazzo
- c: crea la libreria, se non esiste
- s: cerca un indice oggetto-file
  
  Questo flag serve per far in modo che il compilatore crei un indice, il **Symbol table** in cui sono mappate tutte le funzioni contenute all'interno della libreria.

  Operazione che rende piú efficiente l'operazione di linking, perché questo non dovrá scorrere l'intera libreria per trovare la funzione che necessitá ma basta scorrere la symbol table per trovare dove é mappata la specifica funzione all'interno del file oggetto.

NOTA: per convezione il nome della libreira deve iniziare con `lib` e terminare con `.a`

## Collegamento di una libreria statica

Una libreria statica puó esser collegata ad altri file oggetto per creare il file eseguibile.

Il file eseguibile finale includerá anche gli oggetti della libreria statica (overhead in termini di dimensione del file eseguibile).

Il collegamento puó esser fatto tramite questo comando: 

```shell
gcc -o eseg obj1.o ... objN.o -L[] -l[nome libreria]
```

dove: 

- `-L`: indica il path dove ricercare la libreria
- `-l`: specifica il nome della libreria da collegare, epurato dal prefisso `lib` e dal suffisso `.a`.

## Librerie dinamiche

Le librerie dinamiche, o *shared library* sono anche esse collezioni di moduli oggetto.

A differenza delle librerie statiche, le *shared library* sono collegate al programma eseguibile in fase di caricamento, e non in fase di collegamento.

Significa che fintantoché il file non viene eseguito e quindi caricato in memoria RAM, questo non contiene codice associato alla libreria dinamica. Quindi tutti i riferimenti simbolici saranno tradotti in indirizzi unicamente all'atto del caricamento dell'eseguibile in memoria centrale.

In questo modo si risolve il problema dell'overhead in termini di dimensioni del file eseguibile.

- l'eseguibile conterrá solo il codice oggetto che occorre
- se piú eseguibili usano la stessa libreria, il codice oggetto di tale libreria non é duplicato per ogni eseguibile (codice rientrante)



