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

    // inizializziamo la tutto il monitor
    buf->num_lettori = 0;
    // inizializziamo il monitor con 2 condition variable
    init_monitor(&(buf->m), 1);

    int num_lettori = 3;
    int num_scrittori = 3;
    pid_t pid = 0;
    
    for (int i = 0; i < num_scrittori; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            srand(time(NULL)*getpid());
            printf("[SCRITTORE] avvaito\n");
            for (int j = 0; j < 10; j++)
            {
                sleep(1);
                scrittura(buf, rand()% 10);
            }
            exit(0);
        }
    }
    for (int i = 0; i < num_lettori; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("[LETTORE] avvaito\n");
            for (int j = 0; j < 10; j++)
            {
                sleep(2);
                lettura(buf);
            }
            exit(0);
        }
        
    }
    
    


    for (int i = 0; i < num_lettori + num_scrittori; i++)
    {
        wait(NULL);
    }
    
    remove_monitor(&(buf->m));

    return 0;
}