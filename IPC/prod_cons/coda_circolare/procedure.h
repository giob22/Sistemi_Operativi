

// singolo produttore / singolo consumatore

#define SPAZIO_DISP 0 // per gestire l'attesa dei prod

#define MESS_DISP 1 // per gestire l'attesa dei cons
// nel caso in cui abbiamo piú processi consumatori e produttori ho bisogno di gestire il loro accesso agli indici di testa e coda
// altrimenti andremo incontro a RACE CONDITION
#define MUTEX_C 2
#define MUTEX_P 3

// definiamo anche il numero di produttori e consumatori

// tale numero deve essere uguale altrimenti potrebbe capitare che un consumatore o un produttore
// resti in un attesa perenne che un produttore o consumatore effettui la signal

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5

#define DIM_BUFFER 5

struct pool_buffer
{
    int buffer[DIM_BUFFER];
    int testa, coda;
};

/* non va più bene.
void produttore(int *, int);// 1. riferiemnto alla shm(buffer), 2. descrittore del semaforo
void consumatore(int *,int);
*/
void produttore(struct pool_buffer *, int);
void consumatore(struct pool_buffer *, int);
// 1) riferimento alla shm (coda circolare)
// 2) descrittore dell'array semaforico
