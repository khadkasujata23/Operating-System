#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// one mutex for each fork
pthread_mutex_t forks[NUM_PHILOSOPHERS];

// philosopher thread function
void *philosopher(void *arg);

int main() {
  pthread_t philosophers[NUM_PHILOSOPHERS];
  int philosopherId[NUM_PHILOSOPHERS];

  // initialize all forks
  for(int i=0; i<NUM_PHILOSOPHERS; i++)
    pthread_mutex_init(&forks[i], NULL);

  // create philosopher threads
  for(int i=0; i<NUM_PHILOSOPHERS; i++) {
    philosopherId[i] = i;
    pthread_create(&philosophers[i], NULL, philosopher, &philosopherId[i]);
  }

  // wait for all philosophers
  for(int i=0; i<NUM_PHILOSOPHERS; i++)
    pthread_join(philosophers[i], NULL);

  // destroy mutexes
  for(int i=0; i<NUM_PHILOSOPHERS; i++)
    pthread_mutex_destroy(&forks[i]);
  return 0;
}

void *philosopher(void *arg) {
  int id = *(int *)arg;

  // identify left and right forks
  int leftFork = id;
  int rightFork = (id + 1) % NUM_PHILOSOPHERS;

  // thinking
  printf("Philosopher %d is thinking.\n", id);
  sleep(1);

  // pick up left fork
  pthread_mutex_lock(&forks[leftFork]);
  printf("Philosopher %d picked up left fork %d.\n", id, leftFork);

  // pick up right fork
  pthread_mutex_lock(&forks[rightFork]);
  printf("Philosopher %d picked up right fork %d.\n", id, rightFork);

  // eating
  printf("Philosopher %d is eating.\n", id);
  sleep(2);

  // put down right & left fork
  pthread_mutex_unlock(&forks[rightFork]);
  pthread_mutex_unlock(&forks[leftFork]);

  printf("Philosopher %d finished eating.\n", id);
  pthread_exit(NULL);
}