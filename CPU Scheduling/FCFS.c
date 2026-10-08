#include <stdio.h>

typedef struct {
  char process[2];
  float arrivalTime;
  float burstTime;
  float completionTime;
  float waitingTime;
  float Tat;
} Process;

int main() {
  Process p[10];

  int n, i;
  float totalWT = 0, totalTat = 0;
  float currTime = 0;

  printf("Enter number of processes: ");
  scanf("%d", &n);
  for(i=0; i<n; i++) {
    printf("\nEnter process name: ");
    scanf("%s", p[i].process);

    printf("Enter arrival time: ");
    scanf("%f", &p[i].arrivalTime);

    printf("Enter burst time: ");
    scanf("%f", &p[i].burstTime);
  }

  // FCFS Scheduling Calculation
  for (i = 0; i < n; i++) {
    // CPU waits if no process has arrived
    if (currTime < p[i].arrivalTime) {
      currTime = p[i].arrivalTime;
    }

    p[i].completionTime = currTime + p[i].burstTime;
    p[i].Tat = p[i].completionTime - p[i].arrivalTime;
    p[i].waitingTime =  p[i].Tat - p[i].burstTime;

    currTime = p[i].completionTime;

    totalWT += p[i].waitingTime;
    totalTat += p[i].Tat;
  }
  printf("\nProcess\tAT\tBT\tCT\tWT\tTAT\n");
  for (i = 0; i < n; i++) {
    printf("%s\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\n", p[i].process, p[i].arrivalTime, p[i].burstTime, p[i].completionTime, p[i].waitingTime, p[i].Tat);
  }

  printf("\nAverage Waiting Time: %.2f", totalWT / n);
  printf("\nAverage Turnaround Time: %.2f\n", totalTat / n);

  return 0;
}