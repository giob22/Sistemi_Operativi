#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "procedure.h"



int main(){

    key_t k_shm = IPC_PRIVATE;
    int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0664);

    // Abbimo creato la shm, adesso associamoci un indirizzo privato attraverso il quale possiamo accedere

    Buffer* buf = shmat(ds_shm, NULL, 0);

    // inizializziamo tutto il monitor
    buf->num_liberi = DIM_BUFFER;
    buf->num_occupati = 0;
    for (int i = 0; i < DIM_BUFFER; i++)
    {
        buf->stato[i] = VUOTO;
    }
    
    // inizializziamo il monitor con 2 condition variable
    init_monitor(&(buf->m), 2);

    // un volta inizializzato possiamo creare i processi che saranno produttori e consumatori

    int num_processo = 3;
    pid_t pid = 0;
    for (int i = 0; i < num_processo * 2; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            if (i%2 == 0)
            {
                printf("[CONSUMATORE] avviato\n");
                consuma(buf);
            }else{
                printf("[PRODUTTORE] avviato\n");
            
                srand(time(NULL) * getpid());
                int valore = rand()%10;
                produci(buf, valore);
            }
            exit(0);
            
        }else if (pid < 0)
        {
            perror("erorre fork");
            exit(1);
        }
    }

    for (int i = 0; i < num_processo * 2; i++)
    {
        wait(NULL);
    }
    
    remove_monitor(&(buf->m));




    return 0;
}