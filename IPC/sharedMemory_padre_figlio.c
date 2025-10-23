#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    //obiettivi
    // creare una chiave 
    // ottenere il descrittore
    // utilizzare la shm

    // creiamo/otteniamo il descrittore della risorsa
    int *p = NULL;
    key_t shm_key = IPC_PRIVATE;
    //key_t shm_key = ftok(".sharedMemory_padre_figlio.c",'b');
    int ds_shm = shmget(shm_key, sizeof(int), IPC_CREAT | 0664);
    if (ds_shm < 0){
        printf("errore nella creazione della shm");
        exit(-1);
    }

    // colleghiamoci alla risorsa

    p = (int *) shmat(ds_shm,NULL,0); 
    // avenfo inserto NULL al secondo parametro do il compito di trovare un buffer di memoria disponibile da utilizzare per la shared memory
    // al posto di 0 avrei potuto inserire dei flag.
    if(p == NULL){
        printf("errore nella shmat()\n");
        exit(-1);
    }

    printf("dettagli della shm\n");
    printf("chiave IPC: %d\n", shm_key);
    printf("descrittore IPC: %d\n",ds_shm);
    printf("indirizzo della shm dopo l'attach: %p\n",p);

    // creiamo un processo figlio per provare la shared memory appena creata.
    pid_t pid = fork();
    if ( pid == 0 ) {// FIGLIO
        printf("[FIGLIO %d] il contenuto della shm: %d\n",getpid(),*p);
        *p = 123;// cambio il valore contenuto nella shared memory
        exit(0);
    }else if ( pid > 0 ){// PADRE
        wait(NULL);
        printf("[PADRE %d] contenuto della shm: %d\n",getpid(),*p);
    }


    

    // macro la shm come da eliminare per il kernel
    shmctl(ds_shm, IPC_RMID,NULL); // non sará eliminata fin tanto che esiste un processo attached


    return 0;
}