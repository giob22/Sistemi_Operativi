#include "semafori.h"

void wait_sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    // creo l'operazione che deve esser fatta sul semafoto con sembuf
    sem_buf.sem_num = num_sem;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;
    // esegui l'operazione in maniera atomica sul semaforo identificato da sem_num
    // il numero di operazioni è 1
    semop(id_sem, &sem_buf, 1);
}
void signal_sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    sem_buf.sem_num = num_sem;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;

    semop(id_sem, &sem_buf, 1);
}