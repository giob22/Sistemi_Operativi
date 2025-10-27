#include "semafori.h"

int Wait_sem(int id_sem, int numsem){
    // costruiamo la struttura che definisce l'operazione
    struct sembuf sem_buf;
    sem_buf.sem_num = numsem;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;

    // eseguo l'operazione su semaphor set identificato dal descrittore id_sem
    int ret = semop(id_sem, &sem_buf, 1);
    // conviene che gli eventuali errori vengano gestiti
    return ret;
}
int Signal_sem(int id_sem, int numsem){
    // costruiamo la struttura che definisce l'operazione
    struct sembuf sem_buf;
    sem_buf.sem_num = numsem;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;

    // eseguo l'operazione su semaphor set identificato dal descrittore id_sem
    int ret = semop(id_sem, &sem_buf, 1);
    // conviene che gli eventuali errori vengano gestiti
    return ret;
}