#include <iso646.h>
#include <stdio.h>
#include <limits.h>
#define MAX 100

typedef struct {
  int pid;
  int arrTime;
  int burstTime;
  int remTime;
  int compTime;
  int taTime;
  int wTime;
} Process;

// function to calculate tat and wt
void calculateTimes(Process p[], int n);
void display(Process p[], int n);
void inputProcesses(Process p[], int n);

// cpu scheduling algorithms
void SJF(Process p[], int n);
void SRTF(Process p[], int n);

int main() {
  Process p1[MAX], p2[MAX];
  int n;

  printf("Enter no. of processes: ");
  scanf("%d", &n);

  inputProcesses(p1, n);
  for(int i=0; i<n; i++)
    p2[i] = p1[i];

  // Algorithms
  SJF(p1, n);
  SRTF(p1, n);
  return 0;
}


void calculateTimes(Process p[], int n) {
  for(int i=0; i<n; i++) {
    p[i].taTime = p[i].compTime - p[i].arrTime;
    p[i].wTime = p[i].taTime - p[i].burstTime;
  }
}

void display(Process p[], int n) {
  float avgWt = 0, avgTat = 0;
  printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
  for(int i=0; i<n; i++) {
    printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrTime, p[i].burstTime, p[i].compTime, p[i].taTime, p[i].wTime);
    avgWt += p[i].wTime;
    avgTat += p[i].taTime;
  }
  printf("\nAverage Waiting Time: %.2f", avgWt / n);
  printf("\nAverage Turnaround Time: %.2f\n", avgTat / n);
}

void inputProcesses(Process p[], int n) {
  for (int i=0; i<n; i++) {
    p[i].pid= i + 1;
    printf("Enter Arrival Time of P%d: ", i+1);
    scanf("%d", &p[i].arrTime);

    printf("Enter Burst Time of P%d: ", i+1);
    scanf("%d", &p[i].burstTime);
  }
}

void SJF(Process p[], int n) {
  int completed = 0, currTime = 0, visited[MAX] = {0};
  while(completed != n) {
    int shortest = -1;
    int minBurst = INT_MAX;

    // process with shortest burst time
    for(int i=0; i<n; i++) {
      if(!visited[i] && p[i].arrTime <= currTime && p[i].burstTime < minBurst) {
        minBurst = p[i].burstTime;
        shortest = i;
      }
    }

    // if no process has arrived
    if(shortest == -1) {
      currTime++;
      continue;
    }

    // Executing selected process
    currTime += p[shortest].burstTime;
    p[shortest].compTime = currTime;

    visited[shortest] = 1;
    completed++;
  }
  calculateTimes(p, n);
  printf("\n Non-Preemptive SJF Scheduling \n");
  display(p, n);
}

void SRTF(Process p[], int n) {
  int completed = 0, currTime = 0;
  int shortest, minRem;

  // initializing remaining time
  for(int i=0; i<n; i++)
    p[i].remTime = p[i].burstTime;

  while(completed != n) {
    shortest = -1;
    minRem = INT_MAX;

    // Find process with shortest remaining time
    for(int i=0; i<n; i++) {
      if(p[i].arrTime <= currTime && p[i].remTime > 0 && p[i].remTime < minRem) {
        p[i].remTime = p[i].remTime;
        shortest = i;
      }
    }

    // CPU idle
    if(shortest == -1) {
      currTime++;
      continue;
    }

    // executte for one unit of time
    p[shortest].remTime--;
    currTime++;

    // process completed
    if(p[shortest].remTime == 0) {
      completed++;
      p[shortest].compTime = currTime;
    }
  }
  calculateTimes(p, n);
  printf("\n Preemptive SJF (SRTF) Scheduling \n");
  display(p, n);
}