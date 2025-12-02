#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_NUM_THREADS 4

void* print_hello(void* p){ // void* start_routine(void*);
  sleep(3);
  printf("%d: hello world\n", *((int*)p));
  pthread_exit(NULL);
}

int main(){
  pthread_t threads[MAX_NUM_THREADS];

  int ret;

  for(int i = 0; i < MAX_NUM_THREADS; i++){
    printf("%p\n", &i);
    ret = pthread_create(&threads[i], NULL, print_hello, (void *)(&i));
    if (ret != 0){
      printf("ERRORE: return code from pthread_create is %d\n", ret);
      exit(-1);
    }
  }
  double* g = NULL;
  printf("%p\n",&g);
  pthread_exit(NULL); // terminazione del thread padre
}
