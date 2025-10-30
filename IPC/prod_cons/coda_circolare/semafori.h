#include <sys/sem.h>

int wait_sem(int id_sem, int num_sem);
int signal_sem(int id_sem, int num_sem);