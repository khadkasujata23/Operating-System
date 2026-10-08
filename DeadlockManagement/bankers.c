#include <stdio.h>

#define MAX 10

void input(int*, int*, int[][MAX], int[][MAX], int[]);
void calculateNeed(int, int, int[][MAX], int[][MAX], int[][MAX]);
int bankersAlgo(int, int, int[][MAX], int[][MAX], int[], int[]);
void displayNeed(int, int, int[][MAX]);

int main() {
  int n, m;
  int alloc[MAX][MAX], max[MAX][MAX], need[MAX][MAX];
  int avail[MAX], finish[MAX], safeSeq[MAX];

  input(&n, &m, alloc, max, avail);
  calculateNeed(n, m, alloc, max, need);
  displayNeed(n, m, need);

  if(bankersAlgo(n, m, alloc, need, avail, safeSeq)) {
    printf("\nSystem is in safe state.\n");
    printf("Safe Sequence: ");
    for(int i=0; i<n; i++) {
      printf("P%d", safeSeq[i]);
      if(i != n - 1)
          printf(" -> ");
    }
    printf("\n");
  } else {
    printf("System is not in safe state.\n");
  }
  return 0;
}

void input(int *n, int *m, int alloc[][MAX], int max[][MAX], int avail[]) {
  printf("Enter number of processes: ");
  scanf("%d", n);

  printf("Enter number of resource types: ");
  scanf("%d", m);

  printf("Enter Allocation Matrix (%d x %d): \n", *n, *m);
  for(int i=0; i<*n; i++) {
    printf("P%d: ", i);
    for(int j=0; j<*m; j++)
      scanf("%d", &alloc[i][j]);
  }
  printf("Enter Maximum Matrix (%d x %d): \n", *n, *m);
  for(int i=0; i<*n; i++) {
    printf("P%d: ", i);
    for(int j=0; j<*m; j++)
      scanf("%d", &max[i][j]);
  }

  printf("Enter Available Resources (%d values): \n", *m);
  for(int i=0; i<*m; i++)
    scanf("%d", &avail[i]);
}

void calculateNeed(int n, int m, int alloc[][MAX], int max[][MAX], int need[][MAX]) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++)
      need[i][j] = max[i][j] - alloc[i][j];
  }
}

int bankersAlgo(int n, int m, int alloc[][MAX], int need[][MAX], int avail[], int safeSeq[]) {
  int work[MAX], finish[MAX] = {0};

  for(int i=0; i<m; i++)
    work[i] = avail[i];

  int count = 0;
  while(count < n) {
    int found = 0;
    for(int i=0; i<n; i++) {
      if(!finish[i]) {
        int j;
        for(j=0; j<m; j++) {
          if(need[i][j] > work[j])
            break;
        }
        if(j == m) {
          for(int k=0; k<m; k++)
            work[k] += alloc[i][k];

          safeSeq[count++] = i;
          finish[i] = 1;
          found = 1;
        }
      }
    }

    if(!found)
      return 0;
  }
  return 1;
}

void displayNeed(int n, int m, int need[][MAX]) {
  printf("\nNeed Matrix: \n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++)
      printf("%d ", need[i][j]);
    printf("\n");
  }
}