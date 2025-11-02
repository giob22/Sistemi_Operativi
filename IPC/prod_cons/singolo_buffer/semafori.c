#include "semafori.h"

int wait_sem(int id_sem, int num_sem)
{
    // costruiamo l'operazione con sembuf
    struct sembuf sembuf;
    sembuf.sem_flg = 0;
    sembuf.sem_op = -1;
    sembuf.sem_num = num_sem;

    // eseguo l'operazione sul semaphor set identificato da id_sem
    int ret = semop(id_sem, &sembuf, 1);

    return ret;
}
int signal_sem(int id_sem, int num_sem)
{
    // costruiamo l'operazione con sembuf
    struct sembuf sembuf;
    sembuf.sem_flg = 0;
    sembuf.sem_op = 1;
    sembuf.sem_num = num_sem;

    // eseguo l'operazione sul semaphor set identificato da id_sem
    int ret = semop(id_sem, &sembuf, 1);

    return ret;
}