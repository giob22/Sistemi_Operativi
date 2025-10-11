#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>//include diverse tipi di variabile come pid_t: Used for process IDs and process group IDs.
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	int pid;
	pid = fork();
	if(pid < 0){
		perror("fork fallita, valore di ritorno: ");
		exit(1);
	}
	if(pid ==0){
		printf("[FIGLIO] pid: %d\nppid: %d\n", getpid(), getppid());
		
		printf("command line passata dal main: \n");
		for (int i = 0; i< argc; i++){
			printf("%d) %s\n", i, argv[i]);
		}
		
		

		execl("./altroprog_ex","altroprog_ex","4",NULL);
		printf("[FIGLIO] se arrivo a questo punto qualcosa è andato storto, la exec() non ha funzionato\n");
		kill(getpid(), SIGKILL);
		exit(1);
	
	}
		
		
		
	printf("[PADRE]: Sto continuando ad eseguire il codice contenuto in 'main_exec.c' PID: %d\n", getpid());
	
	
	
	
	int status;
	wait(&status);
	printf("[PADRE]Lo stato di terminazione del mio processo figlio è: %d\n",WEXITSTATUS(status));
	printf("%d\n",status);
	
	return 0;
}
