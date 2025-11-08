#include <sys/sem.h>

#define SYNCH 0
#define MUTEXL 1

#define MAX_DIMENSIONE 16

#define NUM_GENERATORI 1
#define NUM_ANALIZZATORI 1
#define NUM_ELABORATORI 2

#define NUM_RIPETIZIONI 5

typedef struct
{
    char testo[MAX_DIMENSIONE];
    int dim; // valore scelto casualmente tra 10 e 15 a tempo di esecuzione
    int numlettori;
    int id_sem;
} Buffer;

void wait_sem(int, int);
void signal_sem(int, int);