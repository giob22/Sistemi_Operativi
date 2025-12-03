#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
// definisco la struttura counter
struct counter{
  int valore;
  pthread_mutex_t mutex;

};

void* in_counter(void* p){
  struct counter* pc = (struct counter*)p;
  for (int i = 0; i < 100000; i++){
    pthread_mutex_lock(&(pc->mutex));
    pc->valore++;
    pthread_mutex_unlock(&(pc->mutex));
  }
  pthread_exit(NULL);
}

int main(){

  pthread_t threads[NUM_THREADS];

  struct counter* p = (struct counter*) malloc(sizeof(struct counter));
  // inizializzo il counter
  p->valore = 0; 
  // inizializzo il mutex
  pthread_mutex_init(&(p->mutex), NULL);
  // creo i thread che faranno gli incrementi
  for (int i = 0; i < NUM_THREADS; i++){
    pthread_create(&threads[i], NULL, in_counter, (void*)p);
  }
  // faccio la join così che il padre si blocchi 
  // in attesa della terminazione dei thread figli
  for (int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  printf("valore raggiunto dal counter → %d\n", p->valore);

  return 0;
}