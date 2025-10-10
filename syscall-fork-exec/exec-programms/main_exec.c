#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
	int pid;
	pid = fork();
	if(pid ==0)
		execl("./altroprog_ex","10","arg1","arg2", NULL);
	printf("Sono il padre e sto continuando ad eseguire il codice contenuto in 'main_exec.c' PID: %d\n", getpid());
	
	
	
	
	int status;
	wait(&status);
	printf("Lo stato di terminazione del mio processo figlio è: %d\n",WEXITSTATUS(status));
	printf("%d\n",status);
	
	return 0;
}
