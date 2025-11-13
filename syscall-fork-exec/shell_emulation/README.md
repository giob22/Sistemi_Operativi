# Simulare una shell

In `shell.c` abbiamo creato una simulazione di una shell reale funzionante per comandi base. <br>

Necessaria è la conoscenza di:

- Processi **backgound** e **foreground**;
- Come vengono passato gli argomenti attraverso la **command line**;
- `fork()` per la creazione di processi e la famigli `exec()` per sostituire la *text-structure* e per deallocare e rimpiazzare i dati globali e statici, lo stack utente e l'heap (il processo mantiene tutte le altre strutture e caratteristiche dopo la `exec()`, come il **PID** e **PPID**);
- `_exit()` per terminare un programma senza che vengano svuotati i buffer;
- `wait(int *)`, funzione che permette al processo padre di sospendersi e aspettare la terminazione del processo figlio per raccogliere lo **stato di terminazione**;
- *stream standard*: stdin (0), stdout, stderr.

## Librerie

Cominciamo quindi alla spiegazione del programma, la prima parte riguarda le librerie include:

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
```

Vediamo a cosa ci sono servite:

 - ```c
    #include <sys/types.h>
    ```

    Contiene la dichiarazione di **tipi di dati fondamentali** usati nelle system call UNIX:
    - `pid_t` → tipo usato per i process ID (PID)

    - `off_t`,`size_t`,`mode_t`, ecc.

- ```c
    #include <sys/wait.h>
    ```
    Contiene la dichiarazione di macro e funzioni per gestire la terminazione dei processi figli:
    - `wait()` e `waitpid()`
    - `WIFEXITED(int)`
    - `WEXITSTATUS(int)`
- ```c
    #include <unistd.h>
    ```
    Contiene dichiarazioni per le **system call POSIX** di basso livello:
    - `close()`;
    - `fork()`; 
    - `_exit()`; 
    - `execvp()`; 
    - `dup2()`; 
    - `read()`; 
    - `write()`; 
    - `getpid()`; 
    - ecc.
- ```c
    #include <stdio.h>
    ```
    Ci permette di utilizzare le funzioni standard di input/output bufferizzato:
    - `print()`;
    - `fprintf()`;
    - `fgets()`;
    - `stdin`;
    - `stderr`;
    - `stdout`;
    - ecc.
- ```c
    #include <string.h>
    ```
    Per funzioni di manipolazione delle stringhe.
- ```c
    #include <fcntl.h>
    ```
    Per l'utilizzo di costanti e dichiarazione da utilizzare con `open()`, come:
    - `O_RDONLY`;
    - `O_WRONLY`;
    - `O_RDWR`
    - ecc.

## Codice: logica

Passiamo alla logica del codice, innanzitutto inizializziamo le variabili necessarie che dovranno contenere gli input che arriveranno da tastiera.

```c
pid_t pid;
int st;
const int NMAX = 50; // numero massimo di parametri
const int CMAX = 100; // numero massimo di caratteri letti dalla sheel
char *argv[NMAX]; //array dei parametri da fornire ad execvp
char comLine[CMAX + 1]; // contiene i comandi forniti da tastiera
char * token; // variabile di supporto per estrarre il comando e i parametri da comLine
char endShell[5] = "quit"; // stringa da digitare per uscire dalla sheel
int n;
```


---

```c
if (fgets(comLine, CMAX + 1, stdin) == NULL){
    //fgets ritorna l'indirizzo del buffer comLine
	fprintf(stderr, "Errore nella lettura degli input dell'utente\n");
	return -1;  
}
```

`fgets()` ha come argomenti il buffer `commLine`, in cui andrà immagazinato il contenuto letto, `CMAX + 1` numero di caratteri da leggere da tastiera, `stdin` da quale *stream* leggere i caratteri (in questo caso tastiera).
Come scritto anche nel commento la funzione restituisce il puntatore all'area di memoria in cui è contenuta la stringa letta, in questo caso `comLine`; se restituisce `NULL` significa che qualcosa è andato storto nella lettura.

---

```c
char * p;
if ((p = strrchr(comLine,'\n')) != NULL){
	*p = '\0';
}
```

`strrchr()` restituisce il puntatore all'area di memoria in cui è contenuta l'ultima occorrenza del carattere specificato, `'\n'`, nella stringa puntata da `comLine`.

---

```c
if(strcmp(comLine, endShell) == 0){
	printf("chiusura della shell\n");
	return 0;
}
```
Dopo aver letto si assicura che l'utente non abbia digitato il comando per uscire dalla shell: "quit".

---

Se non si intende uscire dalla shell allora inizia l'analisi della stringa inserita.

```c
n = 0;
token = strtok(comLine," ");
while(token != NULL){
	argv[n] = token;
	n++;
	token = strtok(NULL, " ");
}
argv[n] = (char *)0;
```

In questa parte di codice si **tokenizza** la stringa contenuta in `comLine`. 

Come funziona `strtok()`:
- la prima volta che viene inserita andiamo a specificare la stringa da tokenizzare e il separatore: `strtok(comLine," ")`. La funzione restituisce il puntatore alla prima stringa separata da `" "`; 
- Le successive chiamate, per ottenere gli altri **token** (le altre parti della stringa separate da `" "`), non dobbiamo specificare nuovamente la stringa di partenza altrimenti si riottiene lo stesso risultato precedente. Quindi utiliziamo `strtok(NULL, " ")`, la funzione ricorda dove si è fermata l'ultima volta. Nel momento in cui finisce la **tokenizzazione** restituisce `NULL`.

Inseriamo ogni token che riceviamo all'interno dell'array di puntatori a stringhe `argv`. Una volta finito il ciclo si inserisce come ultimo elemento dell'array `argv` un puntatore a `NULL` (necessiario per la `exec()`).



---
# continua. . .