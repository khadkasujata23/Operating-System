#include <stdio.h>

#define MAX 50

void input(int *, int *, int[]);
void optimalPageReplacement(int, int, int[]);

int main() {
  int n, frames;
  int pages[MAX];

  input(&n, &frames, pages);
  optimalPageReplacement(n, frames, pages);

  return 0;
}

void input(int *n, int *frames, int pages[]) {
  printf("Enter number of page references: ");
  scanf("%d", n);

  printf("Enter page reference string:\n");
  for (int i = 0; i < *n; i++)
    scanf("%d", &pages[i]);

  printf("Enter number of frames: ");
  scanf("%d", frames);
}

void optimalPageReplacement(int n, int frames, int pages[]) {
  int frame[MAX];
  int pageFaults = 0;

  for (int i = 0; i < frames; i++)
    frame[i] = -1;

  printf("\nPage\tFrames\n");
  for(int i=0; i<n; i++) {
    int found = 0;

    // check if page already exists
    for(int j=0; j<frames; j++) {
      if(frame[j] == pages[i]) {
        found = 1;
        break;
      }
    }

    // page fault occurs
    if(!found) {
      int pos = -1;

      // Find empty frame
      for (int j = 0; j < frames; j++) {
        if (frame[j] == -1) {
          pos = j;
          break;
        }
      } // empty frame (for)

      // if no empty frame, find optimal replacement
      if(pos == -1) {
          int farthest = -1;

          for (int j = 0; j < frames; j++) {
            int nextUse = -1;
            for (int k = i + 1; k < n; k++) {
              if (frame[j] == pages[k]) {
                nextUse = k;
                break;
              }
            }
            // Page not used again
            if (nextUse == -1) {
              pos = j;
              break;
            }
            if (nextUse > farthest) {
              farthest = nextUse;
              pos = j;
            }
        } // optimal for
      } // optimal replacement
      frame[pos] = pages[i];
      pageFaults++;
    } // page fault if
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
  } // outer for close
  printf("\nTotal Page Faults = %d\n", pageFaults);
}