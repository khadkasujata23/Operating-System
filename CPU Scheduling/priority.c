#include <stdio.h>

typedef struct {
  char process[5];
  int arrTime;
  int burstTime;
  int priority;
  int compTime;
  int Wt;
  int Tat;
} Process;

int main() {
  Process p[10], temp;
  int n, i, j;
  int currentTime = 0;
  float totalWaitingTime = 0, totalTurnaroundTime = 0;

  printf("Enter number of processes: ");
  scanf("%d", &n);

  // Input process details
  for (i = 0; i < n; i++) {
    printf("\nEnter process name: ");
    scanf("%s", p[i].process);

    printf("Enter arrival time: ");
    scanf("%d", &p[i].arrTime);

    printf("Enter burst time: ");
    scanf("%d", &p[i].burstTime);

    printf("Enter priority: ");
    scanf("%d", &p[i].priority);
  }

  // Sort processes according to priority
  for (i = 0; i < n - 1; i++) {
    for (j = i + 1; j < n; j++) {
      if (p[i].priority > p[j].priority) {
        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
      }
    }
  }

  // Calculate CT, WT, and TAT
  for (i = 0; i < n; i++) {
    if (currentTime < p[i].arrTime) {
      currentTime = p[i].arrTime;
    }
    p[i].compTime = currentTime + p[i].burstTime;
    p[i].Tat =  p[i].compTime - p[i].arrTime;

    p[i].Wt = p[i].Tat - p[i].burstTime;
    currentTime = p[i].compTime;
    totalWaitingTime += p[i].Wt;
    totalTurnaroundTime += p[i].Tat;
  }

  printf("\nProcess\tAT\tBT\tPriority\tCT\tWT\tTAT\n");

  for (i = 0; i < n; i++) {
    printf("%s\t%d\t%d\t%d\t\t%d\t%d\t%d\n", p[i].process, p[i].arrTime, p[i].burstTime, p[i].priority, p[i].compTime, p[i].Wt, p[i].Tat);
  }

  printf("\nAverage Waiting Time: %.2f", totalWaitingTime / n);
  printf("\nAverage Turnaround Time: %.2f\n", totalTurnaroundTime / n);

  return 0;
}