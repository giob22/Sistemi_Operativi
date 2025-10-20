# Virtualizzazione leggera

**Il Kernel** di un sistema operativo Linux, realizza già una forma di virtualizzazione *leggera*, perché si occupa di dividere e gestire le risorse fisiche (CPU, memoria, dispositivi, ecc.) in modo che ogni processo abbia l'illlusione di possedere un processore tutto suo.

In Linux esiste una struttura dati chiamata *process cotrol block - PCB* che contiene tutte le informazioni necessarie per gestire un processo: stato, registri, memoria, priorità, ecc. . Questo concetto sarà fondamentale quanto parleremo di **virtualizzazione leggera** (come i *container*).

La virtualizzazione leggera differisce da quella *classica*, cioè quella che emula un intero processore. "Emulare" significa **ricreare via software** il comportamento di un processore vero, così da poter far girare un sistema operativo "finto" sopra un altro. Esiste un software chiamato *Hypervisor* che ci permette di avere un'architettura sottostante di un certo tipo e un'architettura da emulare di un altro detta guest. Ciò significa che potremmo eseguire codice ARM su un architettura x86.

Nella virtualizzazione leggera, basata sul sistema operativo, non si crea un altro sistema operativo, ma si usa il **Kernel** stesso per gestire più ambienti isolati. Il kernel non emula hardware, ma divide e assegna le risorse tra processi o container. Possiamo dire che il kernel è un sistema  di **virtualizzazione di risorse**.

## In modo discorsivo:

Nel kernel linux abbiamo già un meccanismo fondamentale chiamato *control block* (es. PCB o altre strutture di controllo) che gestisce ogni processo: lo stato, la memoria, la priorità, i registri, ecc. Quando entreremo nell'argomento della *virtualizzazione leggera*, questo concetto ci sarà utile.

Abbiamo parlato delle tecnologie di **virtualizzazione classiche**: quelle che emulano un'intera macchina fisica, ossia un processore hardware, un'architettura host e una guest. Pensiamo al software *Hypervisor* che consente di far girare codice pensato per un'achiterttura AMR su una macchina x86.

Ma poi c'è un altro livello: la *virtualizzazione leggera* ovvero quella basata sul sistema operativo. In questo caso non si emula tutto l'hardware, bensì si utilizza direttamente il kernel, che divide e assegna le risorse (CPU, memoria, I/O) tra più entità. Qui entrano in gioco i **cgoups**. Il kernel, in questo scenario, non sta emulando un altro processore: sta *dividendo* le risorse tra i processi.

In effetti, ogni processo che il kernel gestisce ha già l'illusione di avere un processore tutto suo: lo scheduler del kernel alterna i processi in modo che ciascuno pensi di avere la CPU dedicata. Quindi possiamo dire che il kernel svolge una forma di virtualizzazione detta *virtualizzazione leggera*, semplicemente attraverso la divisione delle risorse.

Con i *cgroups* stiamo estendendo questo principio.
- raggruppare processi;
- assegnare loro dei limiti;
- monitorare l'utilizzo.

Tutto tramite il kernel. Non emuliamo un'intera macchina ma sfruttiamo gli strumenti che il kernel già ci mette a disposizione per isolare, controllare e allocare risorse.

Quindi quando pensiamo ai **cgroups**, pensiamo a: insieme di processi che ereditano certi limiti dal gruppo genitore, una gerarchia che può esserre configurata (i cgroups non sono tutti sullo stesso livello), controller del kernel che applicano risorse (CPU, memoria, I/O). I **cgroups** sono lo strumento attraverso il quale il kernel trasforma l'idea "ogni processo ha il suo processore" in "ogni gruppo di processi ha la sua fetta di risorse".

In sintesi: il kernel non solo gestisce processi, ma virtualizza risorse — e i cgroups sono il mezzo concreto con cui questa virtualizzazione leggera viene realizzata.