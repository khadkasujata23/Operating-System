#include <stdio.h>

#define MAX 50

void input(int *, int *, int[], int *);
void fifoPageReplacement(int, int, int[]);

int main() {
  int n, pages[MAX];
  int frames;

  input(&n, &frames, pages, &frames);
  fifoPageReplacement(n, frames, pages);

  return 0;
}


void input(int *n, int *frames, int pages[], int *dummy) {
  printf("Enter number of page references: ");
  scanf("%d", n);

  printf("Enter page reference string:\n");
  for (int i = 0; i < *n; i++)
    scanf("%d", &pages[i]);

  printf("Enter number of frames: ");
  scanf("%d", frames);
}

void fifoPageReplacement(int n, int frames, int pages[]) {
  int frame[MAX], front = 0;
  int pageFaults = 0;

  for (int i = 0; i < frames; i++)
    frame[i] = -1;

  printf("\nPage\tFrames\n");

  for (int i = 0; i < n; i++) {
    int found = 0;
    for (int j = 0; j < frames; j++) {
        if (frame[j] == pages[i]) {
            found = 1;
            break;
        }
    }
    if (!found) {
        frame[front] = pages[i];
        front = (front + 1) % frames;
        pageFaults++;
    }
    printf("%d\t", pages[i]);
    for (int j = 0; j < frames; j++) {
      if (frame[j] == -1)
        printf("- ");
      else
        printf("%d ", frame[j]);
    }
    if (found)
      printf("(Hit)");
    else
      printf("(Fault)");
    printf("\n");
  }
  printf("\nTotal Page Faults = %d\n", pageFaults);
}