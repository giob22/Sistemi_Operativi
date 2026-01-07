# Makefile e librerie

Il codice oggetto è la traduzione del sorgente in linguaggio macchina (binario).

I contenuti del codice oggetto sono specifici sia a seconda dell'architettura, sia a seconda del compilatore o linguaggio utilizzata per generarlo.

I codici oggetto si trovano normalmente raggruppati in file oggetto che contengono una serie di funzioni strettamente imparentate tra di loro. Tali file oggetto vengono spesso chiamate librerie, di cui ne esistono diversi tipi:

- statiche 
- dinamiche

Il codice oggetto è composto principalmente da codice eseguibile, più una serie di informazioni che permettono al linker di unirlo, se richiesto, con altri codici oggetto per generare un programma eseguibile.

## indirizzi rilocabili

Il compilatore crea indirizzi rilocabili: registro base + offset.

Il linker ha il compito di fondere i moduli compilati separatamente andando a sostituire gli indirizzi simbolici in indirizzi rilocabili in un unico file oggetto.

Il programma in formato eseguibile contiene: il formato rilocabile, la dimensione, l'entry point.

Il caricatore implementa politiche di rilocazione statica o dinamica:

- statica quando il programma stesso viene caricato in memoria sommando al blocco di memoria virtuale (creato dal processo di traduzione) l'effettivo blocco fisico in modo da potervi accedere
- dinamica invece viene realizzata non durante il caricamento del programma in memoria, ma a *run-time*, da un apposito dispositivo hardware, chiamato MMU.

## Programmazione modulare

Separare la specifica delle funzioni contenute all'interno di un file oggetto, in cui è contenuta l'implementazione di tali specifiche, permette:

- la modifica dell'implementazione del singolo modulo senza che venga alterata l'esecuzione del programma che ne fa utilizzo, a patto che l'interfaccia di programmazione specificata nella intestazione non venga toccata.
- in caso di modifiche non è necessario ricompilare tutto il programma per intero ma ci basta compilare unicamente il modulo che è stato modificato e linkare di nuovo tutto; ci saltiamo la parte di compilazione di tutti gli altri moduli che sono rimasti inalterati dalla modifica.

## Linking

Ogni file può contenere riferimenti simbolici ad altri moduli.

I vari moduli oggetto vengono collegati per produrre un unico modulo caricabile, che non contiene riferimenti simbolici ma unicamente indirizzi rilocabili i quali sono gestiti dal caricatore.

Gli indirizzi simbolici durante il processo di linking vengono trasformati in salti verso i corrispettivi indirizzi rilocabili nel load module complessivo

## Loading

Il loader è responsabile del caricamento in memoria del file eseguibile generato dal linker, il load module.

Ci sono diversi approcci di loading:

- Assoluto
  
  Gli indirizzi sono fissati dal linker, quindi il programma viene caricato sempre nella stessa porzione di memoria.
- Rilocabile
  
  Gli indirizzi sono relativi all'inizio del programma. Il programma può esser caricato in parti diverse della memoria per ogni esecuzione.
- Dinamico
  
  Gli indirizzi nel programma restano logici durante tutta l'esecuzione. La loro traduzione in indirizzi fisici avviene a tempo di esecuzione mediante l'ausilio del componente hardware MMU.

## ELF

`ELF` è il formato file standard sviluppato agli Unix System Laboratories.

Questo formato è utilizzato per file:

- eseguibile
- codice oggetto
- librerie condivise
- code dumps

ELF = Executable and Linkable Format

### File dumping

Possiamo leggere il contenuto di un file in formato ELF utilizzando la primitiva `objdump`.

Tale primitiva permette diversi flag che ci permettono di visualizzare diverse parti del file ELF:

- `-f` mostra i contenuti del file header
- `-p` mostra i contenuti specifici del file 
- `-d` mostra i contenuti in linguaggio macchina
- `-x` mostra i contenuti di tutti gli header

## Make e Makefile

Il comando `make` è un utility usata specialmente per semplificare la compilazione separate di programmi composti da più file sorgente e libreire.

Digitando `make` il comando cerca un file di nome "Makefile" nella directory corrente, lo interpreta ed esegue le regole ivi contenute.

Ogni regola può avere delle dipendenze, che sono dei file da cui il target dipende.

Tali dipendenze possono essere:

- già soddisfatte se il file è presente;
- generare l'esecuzione di un'altra regola all'interno del makefile che come target ha proprio il file.

La modifica di una dipendenza causa la ricostruzione di tutti i target che dipendono da essa.

