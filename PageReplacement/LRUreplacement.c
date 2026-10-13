#include <stdio.h>

#define MAX 50

void input(int *, int *, int[]);
void lruPageReplacement(int, int, int[]);

int main() {
  int n, frames;
  int pages[MAX];

  input(&n, &frames, pages);
  lruPageReplacement(n, frames, pages);

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

void lruPageReplacement(int n, int frames, int pages[]) {
  int frame[MAX], lastUsed[MAX];
  int pageFaults = 0;

  for (int i = 0; i < frames; i++) {
    frame[i] = -1;
    lastUsed[i] = -1;
  }
  printf("\nPage\tFrames\n");
  for(int i=0; i<n; i++) {
    int found = 0;

    // Check if page is already in a frame
    for (int j = 0; j < frames; j++) {
      if (frame[j] == pages[i]) {
        found = 1;
        lastUsed[j] = i;
        break;
      } // if close
    } // for close inner

    if(!found) {
      int pos = 0;

      // Find an empty frame
      for (int j = 0; j < frames; j++) {
        if (frame[j] == -1) {
          pos = j;
          goto replace;
        } // if close
      } // for close

      // Find least recently used page
      for(int j=1; j<frames; j++) {
        if(lastUsed[j] < lastUsed[pos])
          pos = j;
      }

      replace:
        frame[pos] = pages[i];
        lastUsed[pos] = i;
        pageFaults++;

    } // if(!found) close

    printf("%d\t", pages[i]);
    for(int j=0; j<frames; j++) {
      if(frame[j] == -1)
        printf("- ");
      else
        printf("%d ", frame[j]);
    }

    if(found)
      printf("(Hit)");
    else
     printf("(Fault)");
    printf("\n");
  } // main for close
  printf("Total Page Faults = %d\n", pageFaults);
}