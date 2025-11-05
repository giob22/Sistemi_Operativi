#define MUTEXL 0
#define SYNCH 1

typedef struct 
{
    int num_lettori;
    long int msg;
} Buffer;

void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);