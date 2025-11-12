#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "semafori.h"


int main(){
	// inizzializzo un semaforo che verrá condiviso tra due processi
	key_t k_sem = ftok("./programma1_exe", 'b');
	printf("chiave: %d\n", (int)k_sem);

	// dopo aver ottenuto la chiave dell'array semaforico ottengo il costruttore
	// che dovrebbe già esser creato dal programma1
	
	int ds_sem = semget(k_sem, 1, IPC_CREAT | IPC_EXCL | 0664);
	printf("valore di ds_sem: %d\n", ds_sem);
	if (ds_sem < 0){
		printf("il semaforo é stato creato correttamente dal programma1.c\n");
		ds_sem = semget(k_sem, 1, 0);
		printf("ho ottenuto il semaforo esistente\n");
		printf("[PROGRAMMA 2] il descrittore del semaforo é: %d\n", ds_sem);
	}else{
		semctl(ds_sem, 0, IPC_RMID),
		printf("il semaforo non é stato creato, ERRORE\n");
		exit(1);
	}


	// potrei utilizzare il semaforo, ma non lo faccio, attendo 10 secondi per vedere cosa accade
	// se il programma1.c lo marca come eliminabile.
	// per vedere se é stato eliminato in questo arco di tempo controllo ipcs per vedere
	// se esiste ancora il semaforo
	// prima di attendere risveglio il semaforo il programma precedente in attesa sul semaforo
	// cosí piú nessun processo é sulla cosa di attesa di questo
	// struct sembuf op;
	// op.sem_num = 0;
	// op.sem_op = 1;
	// op.sem_flg = 0;
	
	// semop(ds_sem, &op, 1);
	
	
	Signal_sem(ds_sem, 0);
	sleep(10);

	// faccio eseguire una exec ad un processo figlio per verificare se il semaforo esiste ancora
	printf("[PROGRAMMA 2]eseguo una ipcs per verificare a terminale se è presente un semaforo\n");
	pid_t pid = fork();
	if (pid == 0)
	{
		execl("/usr/bin/ipcs", "ipcs","-s", (char* )NULL);
		printf("ERRORE EXEC");
		exit(0); // nel caso di errore dell'exec
	}
	wait(NULL);
	


	// controllo ipcs
	// controllando ipcs dopo che il programma1 è stato sbloccato notiamo che il semaforo non esiste più
	// questo significa che il programma 2 se avesse fatto qualche altra operazione sul semaforo questa avrebbe
	// restituito un essore, proprio perché il semaforo è stato cancellato

	// facciamo una prova, tentiamo di ottenere informazioni sul semaforo come sem_val
	// mediante l'utilizzo di semctl
	int valore_sem = semctl(ds_sem, 0, GETVAL);
	printf("[PROGRAMMA 2]Ottengo il valore di sem_val per il semaforo con id_sem = %d (-1 indica che il semaforo non esiste): %d\n", ds_sem,valore_sem);
	printf("FINE 2\n");
	
	return 0;
}
