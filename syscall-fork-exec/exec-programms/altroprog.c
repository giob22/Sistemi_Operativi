#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int N = atoi(argv[1]);
	//all’interno di ls, argv[0] sarebbe "pippo" — ma il programma funzionerebbe ugualmente, solo che nei messaggi di errore o di help ls si identificherebbe come pippo.
	
	int pid = getpid();

	printf("Sono altroprog e sono in esecuzione con PID: %d\nPPID: %d\n",pid,getppid());

	for(int i = 0; i < N; i++){
		printf("%d\n",i+1);
		sleep(1);
	}
	exit(55);

	return 0;
}	
