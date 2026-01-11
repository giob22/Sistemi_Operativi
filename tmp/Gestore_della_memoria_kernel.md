# Allocatore kernel level

Il kernel contiene a sua volta un gestore della memoria fisica.

Tale allocatore ci permette di allocare pagine di memoria in memoria centrale per i processi utente e per il kernel stesso.

Il gestore della memoria a livello kernel é composto da 3 allocatori:

- slab allocator: gestisce l'allocazione **a porzioni di pagina**
- zone allocator: gestisce l'allocazione **a multipli di pagina**
- buddy system: alloca pagine in una zona

## Zone allocator

Seleziona la zona in cui allocare le pagine richieste.

Abbiamo principalmente 3 tipi diversi di zone nella memoria principale

- zona DMA
  
  memoria fisica riservata per accedere ai dispositivi legacy che sfruttano un parallelismo ad 8 o 16 bit
- zona NORMAL
  
  memoria fisica accessibile velocemente dal codice del kernel

  Zona in cui si **collocano i dati interni del kernel** ed é associata in maniera statica alla parte iniziale della memoria fisica

- zona HIGH MEMORY
  
  memoria fisica accessibile lentamente, per cui é necessaria la traduzione da parte della MMU perché viene allocata in maniera dinamica.

  infatti é utilizzata per allocare dinamicamente pagine del kernel e pagine corrispondenti alle VMA dei processi.

## Buddy system

Consiste nell'algoritmo che viene utilizzato per allocare gruppi di pagine fisiche contigue.

Nonostante di abbia la possibilitá di utilizzare un tipo di allocazione non contigua si tenta di allocare pagine di uno stesso blocco in modo contiguo per aumentare le performance.

meno cache miss, tabella delle pagine ridotta, compatibilitá con DMA e dispositivi di I/O

Una allocazio avviene per **multipli** **di \\(2^k\\) pagine contigue**. 

Il kernel traccia il blocco libero tramite una linked list

## slab allocator

Lo slab allocator permette di allocare porzioni piccole di memoria, di dimensione variabile.

Tali porzioni sono minori di quella di una pagina.

Alloca **pagine con oggetti pre-allocati di stessa dimensione**.

Utilizzato per allocare strutture dati contenenti informazioni che frequentemente vengono modificate.

Un esempio potrebbero essere le task_struct dei processi all'interno di una unica pagina in memoria fisica.

## Page frame reclaiming alorithm

L'algoritmo di rimpiazzo delle pagine (PFRA) é una variante dell'algoritmo second chance

Viene eseguito nel momento in cui:

- La memoria libera é al di sotto di una soglia
- Periodicamente

Sfrutta due liste concatenate di pagine, una detta Attive l'altra Inattive.


