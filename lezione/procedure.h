

// singolo produttore / singolo consumatore

#define SPAZIO_DISP 0 // per gestire l'attesa dei prod

#define MESS_DISP 1	// per gestire l'attesa dei cons

#define MUTEX_PROD 2
#define MUTEX_CONS 3

#define NUM_CONSUMATORI 5
#define NUM_PRODUTTORI 5 // il numero deve essere uguale altrimenti esistera un consumatore o un produttore in attesa perenne


#define DIM_BUFFER 5

struct pool_buffer
{
    // pool
    int buffer[DIM_BUFFER];
    int testa, coda;
};


/* non va più bene. 
void produttore(int *, int);// 1. riferiemnto alla shm(buffer), 2. descrittore del semaforo
void consumatore(int *,int);
*/
void produttore(struct pool_buffer *, int);// 1. riferiemnto alla shm(buffer), 2. descrittore del semaforo
void consumatore(struct pool_buffer *,int);
