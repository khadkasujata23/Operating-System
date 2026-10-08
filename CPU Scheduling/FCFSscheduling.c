#include <stdio.h>

int main() {
  int n, head, i;
  int request[100];
  int totalSeek = 0;

  printf("Enter number of disk requests: ");
  scanf("%d", &n);

  printf("Enter disk request sequence:\n");
  for (i = 0; i < n; i++)
    scanf("%d", &request[i]);

  printf("Enter initial head position: ");
  scanf("%d", &head);

  printf("\nOrder of servicing:\n");

  for (i = 0; i < n; i++) {
    int distance = request[i] - head;
    if (distance < 0)
        distance = -distance;
    totalSeek += distance;
    printf("%d -> %d\n", head, request[i]);
    head = request[i];
  }

  printf("\nTotal Seek Time = %d\n", totalSeek);
  printf("Average Seek Time = %.2f\n", (float)totalSeek / n);

  return 0;
}