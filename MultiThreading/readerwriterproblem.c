#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2

// shared resources
int sharedData = 0;
int readCount = 0; // active readers

sem_t writeLock; // semaphore for writers
pthread_mutex_t mutex; // mutex to protect readCount

// Reader and writer thread
void *reader(void *arg);
void *writer(void *arg);

int main() {
  pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
  int readerId[NUM_READERS], writerId[NUM_WRITERS];

  // initialize semaphore and mutex
  sem_init(&writeLock, 0, 1);
  pthread_mutex_init(&mutex, NULL);

  // reader threads
  for (int i=0; i<NUM_READERS; i++) {
    readerId[i] = i+1;
    pthread_create(&readers[i], NULL, reader, &readerId[i]);
  }

  // writer threads
  for (int i=0; i<NUM_WRITERS; i++) {
    writerId[i] = i+1;
    pthread_create(&writers[i], NULL, writer, &writerId[i]);
  }

  // wait for all readers
  for (int i = 0; i<NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
  }

  // wait for all writers
  for (int i = 0; i<NUM_WRITERS; i++) {
    pthread_join(writers[i], NULL);
  }

  // Destroy semaphore and mutex
  sem_destroy(&writeLock);
  pthread_mutex_destroy(&mutex);

  return 0;
}

void *reader(void *arg) {
  int id = *(int *)arg;
  // entry
  pthread_mutex_lock(&mutex);
  readCount++;
  if(readCount == 1)
    sem_wait(&writeLock); // first reader blocks writers
  pthread_mutex_unlock(&mutex);

  // critical region (reading)
  printf("Reader %d is reading: %d\n", id, sharedData);
  sleep(1);

  // exit section
  pthread_mutex_lock(&mutex);
  readCount--;

  if(readCount == 0)
    sem_post(&writeLock); // last reader allows writers
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

void *writer(void *arg) {
  int id = *(int *)arg;

  // wait until no reader is reading
  sem_wait(&writeLock);

  // critical region (writing)
  sharedData++;
  printf("Writer %d updated data to: %d\n", id, sharedData);
  sleep(1);

  // release the resource
  sem_post(&writeLock);
  pthread_exit(NULL);
}