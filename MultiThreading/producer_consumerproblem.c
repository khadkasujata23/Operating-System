#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 10

// Shared buffer
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Semaphores
sem_t empty, full;

// Mutex lock
pthread_mutex_t mutex;

// Function prototype
void *producer(void *arg);
void *consumer(void *arg);

int main() {
  pthread_t producerThread, consumerThread;

  // initialization of semaphores
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  // initialization mutex
  pthread_mutex_init(&mutex, NULL);

  // creating threads
  pthread_create(&producerThread, NULL, producer, NULL);
  pthread_create(&consumerThread, NULL, consumer, NULL);

  // wait for threads to finish
  pthread_join(producerThread, NULL);
  pthread_join(consumerThread, NULL);

  // destroy mutex & semaphores
  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}

// Producer Thread
void *producer(void *arg) {
  int item;
  for(int i=1; i<=TOTAL_ITEMS; i++) {
    item = i;
    // wait if buffer is full
    sem_wait(&empty);

    // enter critical region
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    printf("Producer produced: %d\n", item);
    in = (in + 1) % BUFFER_SIZE;

    // exit critical region
    pthread_mutex_unlock(&mutex);

    // increase no. of full slots
    sem_post(&full);
    sleep(1);
  }
  pthread_exit(NULL);
}

// Consumer Thread
void *consumer(void *arg) {
  int item;
  for (int i=0; i<=TOTAL_ITEMS; i++) {
    // wait if buffer is empty
    sem_wait(&full);

    // enter critical region
    pthread_mutex_lock(&mutex);
    item = buffer[out];
    printf("Consumer consumed: %d\n", item);

    out = (out + 1) % BUFFER_SIZE;
    // exit critical region
    pthread_mutex_unlock(&mutex);

    // increase no. of empty slots
    sem_post(&empty);
    sleep(2);
  }
  pthread_exit(NULL);
}