#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *threadFunc(void *arg);

int main() {
  pthread_t threads[NUM_THREADS];
  int threadIds[NUM_THREADS];

  // create threads
  for(int i=0; i<NUM_THREADS; i++) {
    threadIds[i] = i+1;
    int result = pthread_create(&threads[i], NULL, threadFunc, &threadIds[i]);
    if(result != 0) {
      printf("Error creating thread %d\n", i+1);
      exit(1);
    }
  }

  // waiting for threads to complete
  for(int i=0; i<NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("All threads have completed execution\n");

  return 0;
}

void *threadFunc(void *arg) {
  int threadId = *(int *) arg;
  printf("Thread %d is running\n", threadId);

  // thread task
  for(int i=1; i<=5; i++) {
    printf("Thread %d: Count = %d\n", threadId, i);
  }
  printf("Thread %d finished executing\n\n", threadId);
  pthread_exit(NULL);
}
