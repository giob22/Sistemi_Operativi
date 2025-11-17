#define MUTEX 0
// per gestire l'accesso in mutua esclusione alla risorsa condivisa da parte degli scrittori
#define MUTEXS 1
// per gestire l'accesso alla variabile num_scrittori in mutua esclusione, da parte degli scrittori
#define MUTEXL 2
// per gestire l'accesso alla varaibile num_lettori in mutua esclusione, da parte dei lettori
#define SYNCH 3
// per garantire la mutua esclusione tra i processi lettori e scrittori

typedef struct 
{
    int num_scrittori, num_lettori;
    long int messaggio;
}Buffer;

void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);


