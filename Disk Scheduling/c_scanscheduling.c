#include <stdio.h>

#define MAX_REQUESTS 100

void sortRequests(int[], int);
void cscan(int[], int, int, int);

int main() {
  int n, head, size;
  int request[MAX_REQUESTS];
  int i;

  printf("Enter number of requests: ");
  scanf("%d", &n);

  printf("Enter request sequence:\n");
  for (i = 0; i < n; i++)
    scanf("%d", &request[i]);

  printf("Enter initial head position: ");
  scanf("%d", &head);

  printf("Enter disk size: ");
  scanf("%d", &size);

  sortRequests(request, n);

  cscan(request, n, head, size);
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

void cscan(int request[], int n, int head, int size) {
  int current = head;
  int totalSeek = 0;
  int i;

  printf("\nOrder of servicing:\n");

  // Service requests on the right of the head
  for (i = 0; i < n; i++) {
    if (request[i] >= head) {
      printf("%d -> %d\n", current, request[i]);
      totalSeek += request[i] - current;
      current = request[i];
    }
  }

  // Move to the last cylinder
  if (current != size - 1) {
    totalSeek += (size - 1) - current;
    current = size - 1;
  }

  // Jump to cylinder 0
  totalSeek += size - 1;
  current = 0;

  // Service remaining requests
  for (i = 0; i < n; i++) {
    if (request[i] < head) {
      printf("%d -> %d\n", current, request[i]);
      totalSeek += request[i] - current;
      current = request[i];
    }
  }

  printf("\nTotal Seek Time = %d\n", totalSeek);
  printf("Average Seek Time = %.2f\n", (float)totalSeek / n);
}