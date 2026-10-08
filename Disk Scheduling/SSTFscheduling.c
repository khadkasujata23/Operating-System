// Shortest Seek Time First (SSTF)
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REQ 100

int findClosestReq(int[], int[], int, int);

int main() {
  int req[MAX_REQ], visited[MAX_REQ] = {0};
  int n, head, totalSeekTime = 0;

  printf("Enter the no. of disk requests: ");
  scanf("%d", &n);

  printf("Enter the disk request sequence: ");
  for(int i=0; i<n; i++)
    scanf("%d", &req[i]);

  printf("Enter the initial head position: ");
  scanf("%d", &head);

  printf("\nOrder of servicing requests: \n");
  for(int i=0; i<n; i++) {
    int index = findClosestReq(req, visited, n, head);
    int seekTime = abs(head-req[index]);
    totalSeekTime += seekTime;
    printf("%d -> ", req[index]);
    head = req[index];
    visited[index] = 1;
  }

  printf("End\n");

  printf("\nTotal Seek Time = %d\n", totalSeekTime);
  printf("Average Seek Time = %.2f\n", (float)totalSeekTime/n);

  return 0;
}

int findClosestReq(int requests[], int visited[], int n, int currentHead) {
  int minDist = INT_MAX, index = -1;

  for(int i=0; i<n; i++) {
    if(!visited[i]) {
      int distance = abs(currentHead - requests[i]);
      if(distance < minDist) {
        minDist = distance;
        index = i;
      }
    }
  }

  return index;
}