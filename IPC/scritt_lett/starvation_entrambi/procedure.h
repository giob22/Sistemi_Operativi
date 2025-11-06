#define MUTEX 0
#define MUTEXS 1
#define MUTEXL 2
#define SYNCH 3

typedef struct 
{
    int num_scrittori, num_lettori;
    long int messaggio;
}Buffer;

void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);


