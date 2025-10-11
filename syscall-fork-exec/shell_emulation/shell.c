#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
	pid_t pid;
	int st;
	const int NMAX = 50; // numero massimo di parametri
	const int CMAX = 100; // numero massimo di caratteri letti dalla sheel
	char *argv[NMAX]; //array dei parametri da fornire ad execvp
	char comLine[CMAX + 1]; // contiene i comandi forniti da tastiera
	char * token; // variabile di supporto per estrarre il comando e i parametri da comLine
	char endShell[5] = "quit"; // stringa da digitare per uscire dalla sheel

	int n;


	printf("\n     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n");
	printf("\n     --    Questo programma simula il funzionamento di una shell    --\n");
	printf("\n     ^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n");
	printf("\n                        * Per uscire digita quit *                    \n"); 

	while(1){
		printf("my-shell>> ");

		//legge una riga dell'utente; esce dalla shell in caso di errore
		if (fgets(comLine, CMAX + 1, stdin) == NULL){		//fgets ritorna l'indirizzo del buffer comLine
			fprintf(stderr, "Errore nella lettura degli input dell'utente\n");
			return -1;
		}


		// rimuoviamo il ritorno a capo dalla stringa letta -> trasformiamo "cioa\n\0" in "ciao\0\0"
		char * p;
		if ((p = strrchr(comLine,'\n')) != NULL){ // strrchr trova l'ultima occorrenza del carattere '\n' e restituisce il puntatore a quel carattere
			*p = '\0';
		}

		//esce dalla shell se l'utente scrive il comando 'quit'
		if(strcmp(comLine, endShell) == 0){
			printf("chiusura della shell\n");
			return 0;
		}


		//altrimenti si analizza la stringa inserita dall'utente 
		// strtok serve per dividere(tokenizzare) una stringa in più parti usando dei separatori
		n = 0;
		token = strtok(comLine," ");
		while(token != NULL){
			argv[n] = token;
			n++;
			token = strtok(NULL, " ");
		}
		argv[n] = (char *)0;		//l'ultimo argomento della execv deve puntare a null

		//consideriamo il caso in cui l'utente abbia solo digitato invio, quindi nessun comando 
		//questo significa che comLine contiene solo "\n" -> "\0" quindi NULL
		//di conseguenza anche argv[0] contiene NULL
		if (argv[0] == NULL){
			continue;	// è una parola chiave del linguaggio C e salta direttamente al prossimo ciclo del loop, quindi le successive istruzioni sono bypassate.
		}

		//cominciamo a creare il processo figlio che dovrà eseguire il comando digitato dell'utente.

		pid = fork();
		if (pid < 0){
			fprintf(stderr,"generazione del processo fallita\n");
			return -1;
		}
		if(pid == 0){
			//stiamo eseguendo sul processo figlio
			execvp(argv[0],argv); // in argv[0] dovrebbe esser contenuto il nome del comando da esegiure e successivamente passiamo l'intero array contenente i parametri passati da command line
			fprintf(stderr,"%s: comando non trovato\n",argv[0]);
			_exit(1); // fa una exit() ma non svuota i buffer. Altrimenti il figlio potrebbe svuotare i buffer che sono stati duplicati dal padre.
					  // ad esempio potrebbe scrivere sul terminale.

		}
		else if(pid > 0){
			wait(&st);

			if(WIFEXITED(st) && WEXITSTATUS(st) != 0)
				printf("processo terminato con stato negativo (%d)\n",st);
			
		}


	
	}




	return 0;
}
