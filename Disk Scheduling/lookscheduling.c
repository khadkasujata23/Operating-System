#include <stdio.h>
#include <stdlib.h>


#define MAX_REQUESTS 100

void sortRequests(int[], int);

int main() {
  int requests[MAX_REQUESTS];
  int n, head;
  int totalSeekTime = 0;

  printf("Enter the number of disk requests: ");
  scanf("%d", &n);

  printf("Enter the disk request sequence:\n");
  for(int i=0; i<n; i++)
    scanf("%d", &requests[i]);

  printf("Enter the initial head position: ");
  scanf("%d", &head);

  sortRequests(requests, n);

  int start = 0;
  while(start < n && requests[start] < head)
    start++;

  printf("\nOrder of servicing requests: \n");
  // Move towards higher track numbers
  for (int i = start; i < n; i++) {
      totalSeekTime += abs(head - requests[i]);
      printf("%d -> ", requests[i]);
      head = requests[i];
  }

  // reverse direction and service remaining requests
  for (int i=start-1; i >= 0; i--) {
    totalSeekTime += abs(head-requests[i]);
    printf("%d -> ", requests[i]);
    head = requests[i];
  }
  printf("End\n");

  printf("\nTotal Seek Time = %d\n", totalSeekTime);
  printf("Average Seek Time = %.2f\n", (float)totalSeekTime / n);

  return 0;
}

void sortRequests(int requests[], int n) {
  for (int i=0; i<n-1; i++) {
    for(int j=0; j<n-i-1; j++) {
      if(requests[j] > requests[j+1]) {
        int temp = requests[j];
        requests[j] = requests[j+1];
        requests[j+1] = temp;
      }
    }
  }
}