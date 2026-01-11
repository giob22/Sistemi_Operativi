# de-privileging

il de-privileging é la tecnica fondamentale che permette alla virtualizzazione classica di funzionare.

in poche parola, consite nel togliere al sistema operativo Guest i veri diritti di comando sull'hardware.

Ovvero far eseguire il sistema operativo Guest ad un livello di privilegio in cui non é pensato per girare. Ovvero in user mode.

Quindi il sistema operativo Guest non é il controllore delle risorse hardware, é illuso di esserlo, questo infatti gira in virtuale kernel mode ovvero sempre nella user mode.

Invece ad avere i permessi per girare in kernel mode é il VMM che ha il compito di intercettare le istruzioni sensibili, che necessitano di essere emulate per il sistema operativo Guest.

Questo meccanismo di emulazione degli effetti di una istruzione critica é possibile sfruttando le trap che il processore fisico solleva nel momento in cui si tenta di eseguire delle istruzioni privilegiate. Tali trap vengono intercettate dalla VMM in modo che questa possa emularne l'effetto per illudere al SO Guest che il processore virtuale abbia eseguito l'istruzione sensivite.

Questo meccanismo che sfrutta le trap é detto, TRAP-AND-EMULATE.

Quando la trap viene intercettata dall'Hypervisor, questo elabora l'istruzione che ha generato la trap ed emula l'istruzione privilegiata associata.

## differenza tra istruzioni sensitivi e istruzioni privilegiate

le istruzioni sensitivi sono istruzioni che modificano o dipendono dalla configurazione dell'hardware.

ovvero sono le istruzioni che interagiscono direttamente con la configurazione o lo stato dell'hardware.

- modifica della modalitá del processore
- modifica degli stati dell'hardware
- istruzioni il cui comportamento é diverso in modalitá utente/kernel

le istruzioni privilegiate sono quelle che scatenano una trap quando eseguite in user mode.

per il teorema di popek ew goldbearg una macchina puó esser virtualizzata mediante il meccanismo di trap-and-emulate unicamente se tutte le istruzioni sensitivi sono anche istruzioni privilegiate.

## dynamic bynary traslation

il salto finale viene modificato con un salto al VMM in modo che questo possa controllare il prossimo basic block che il guest OS aveva intenzione di eseguire, per verificare che questo sia sicuro, quindi giá tradotto, o meno.

Quindi il salto serve a garantire che il guest OS non possama mai eseguire un blocco di istruzioni che non sia stato prima controllato dal VMM. Per questo motivo per ogni basic block il controllo deve ritornare al VMM, per far in modo che questo possa controlla il prossimo basic block a cui il guest OS avrebbe voluto saltare.

## virtualizzazione della memoria

All'interno di una VM, il guest os gestisce la finta memoria fisica della VM

Il VMM amministra la vera memoria fisica, anche esso con paginazione a domanda, swapping, algoritmo di prelazione

Le VM sono isolate e non possono accedere alla memoria di altre VM, come accade per i processi in esecuzione sul sistema operativo

### shadow page table

É una tecnica software per la virtualizzazione della memoria.

Il VMM intercetta le modifiche alle page table nel guest, creando una loro controparte detta shadow page table

La CPU usa la shadow page table, ignorando la page table del guest os

Quando il guest os scrive, con un istruzione sensitive, nel registro PTBR l'indirizzo della page table

Con trap and emulate, il VMM carica nel registro (fisico) PTBR l'indirizzo della shadow page table.

Il VMM riesce ad intercettare le modifiche della page table da parte del guest os perché il VMM ha reso le pagine in cui é allocata la page table read-only, quindi ogni tentativo di modifica causa una page foult

Il VMM per ogni page table costruita dal sistema operativo guest deve mantenere la corrispettiva shadow page table.

Sono un meccanismo di virtualizzazione oneroso in quanto aumentano di molto la quantitá di page foult.

questi page foult possono essere genuini e dovuti al meccanismo del caricamento a domanda del sistema operativo guest, oppure dovuti a modifiche della page table.

Anche in questo caso con il tempo sono stati introdotti supporti hardware che hanno permesso di evitare la gestione di un elevato overhead.

Utilizzo delle Extendend page tables, gestite dalla VMM

Queste extendend page table aggiungono un livello di traduzione degli indirizzi. L'MMU accede in hardware ad entrambe le tabelle.

si evita il meccanismo di hypervisor induced page foults.

## virtualizzazione dell'IO

Le operazioni di IO sono virtualizzate con trap and emulate

Si simula un dispositivo virtuale

possono essere emulati entrambi i meccanismi di indirizzamento per i registri del controller IO

Sia memory-mappedIO che port-basedIO

memory mapped intercetta accessi agli indirizzi del dispositivo tramite page foult

port based si intercettano le istruzioni sensibili di IO

Tecniche per la virtualizzazione dell'IO

full virtualization

para virtualization

IO passthrough

SR-IOV