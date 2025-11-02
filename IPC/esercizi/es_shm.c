#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// creo una shm attraverso una chiave generata con ftok, così
	// che nel momento in cui eseguo più volta lo stesso programma
	// posso accedervi senza problemi

	key_t k_shm = ftok("./es_shm.c", 's');

	// otteniamo il descrittore
	// nel crearlo verifichiamo anche se non sia già esistente
	// e quindi ci basta collegarci

	int ds_shm = shmget(k_shm, 100, IPC_CREAT | IPC_EXCL | 0644);
	if (ds_shm < 0)
	{	// la risorsa con tale chiave esiste già
		// mi basta connettermi
		ds_shm = shmget(k_shm, 100, 0);
	}

	// abbiamo creato/ottenuto il descrittore di una shm (IPC),
	// ci manca fare l'attach a questa

	char *ptr = NULL;
	ptr = (char *)shmat(ds_shm, 0, 0);
	if (ptr == NULL)
	{
		printf("errore durante la attach\n");
	}

	// a questo punto implementiamo le funzioni di lettura e scrittura
	// da terminale
	if (argc > 1)
	{
		if (strcmp(argv[1], "r") == 0)
		{
			printf("%s\n", ptr);
		}
		else if (strcmp(argv[1], "w") == 0)
		{
			if (argc > 2)
			{
				strcpy(ptr, argv[2]);
			}
			else
			{
				strcpy(ptr, "\0");
			}
		}
		else if (strcmp(argv[1], "d") == 0)
		{
			printf("eliminazione della shm...\n");
			shmctl(ds_shm, IPC_RMID, NULL);
		}
	}
	else
	{
		printf("errore\n");
		exit(-3);
	}

	return 0;
}
