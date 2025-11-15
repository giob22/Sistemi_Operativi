#include <stdio.h>//
#include <unistd.h>//
#include <stdlib.h> 
#include <sys/wait.h>
#include <time.h> // per la funzione sleep() che non è presente ma abbiamo usato
		  // per mettere in sospeso il processo per visualizzarlo attraverso
		  // il comando ps aux su terminale.

int main(){
	int pid, N = 3;

	//pid = fork();
	//creo un nuovo processo e identifico quale dei due è padre e quale sia il figlio:
	//if (pid > 0){
	//	pid = getpid();
	//	printf("Sono il processo PADRE con PID: %d\n",pid);
	//}else if (pid == 0){
	//	pid = getpid();
	//	printf("Sono il processo FIGLIO con PID: %d\n",pid);
	//}

	//creo un numero N di figli (ATTENZIONE una brutta implementazione potrebbe portare alla classica FORKBOMB, fai attenzione ad evitare che dopo la creazione ogni figlio indipendetemente continui a creare altri figli a sua volta))
	for (int i = 0; i < N; i++){
		pid = fork();
		if (pid == 0){
			printf("sono il figlio numero %d, e ho un PID: %d\n", i, getpid());
			i = N;
			exit(55); //aggiumgiamo che i figli terminino con un certo stato di terminazione.
				  //vogliamo che il padre raccolga lo stato di terminazione dei propri figli
		}
	}

	int status;
	if (pid > 0){
		printf("Sono il padre con PID: %d\n",getpid());
		for (int i = 0; i < N; i++){
			wait(&status);
			printf("un figlio è terminato con stato: %d\n", WEXITSTATUS(status));
		}
	}
	// a cosa serve WEXITSTATUS(int status)
	return 0;
}
		
