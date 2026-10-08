#include <stdio.h>
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

void inputProcesses(Process p[], int n);
void calculateTimes(Process p[], int n);
void display(Process p[], int n);
void roundRobin(Process p[], int n, int quantum);

int main() {
  Process p[MAX];
  int n, quantum;

  printf("Enter no. of processes: ");
  scanf("%d", &n);
  inputProcesses(p, n);

  printf("\nEnter Time Quantum: ");
  scanf("%d", &quantum);

  roundRobin(p, n, quantum);
  return 0;
}

void inputProcesses(Process p[], int n) {
  for(int i = 0; i < n; i++) {
    p[i].pid = i + 1;

    printf("\nEnter Arrival Time of P%d: ", i + 1);
    scanf("%d", &p[i].arrTime);

    printf("Enter Burst Time of P%d: ", i + 1);
    scanf("%d", &p[i].burstTime);

    p[i].remTime = p[i].burstTime;
  }
}

void calculateTimes(Process p[], int n) {
  for(int i = 0; i < n; i++) {
    p[i].taTime = p[i].compTime - p[i].arrTime;
    p[i].wTime = p[i].taTime - p[i].burstTime;
  }
}

void display(Process p[], int n) {
  float avgWT = 0;
  float avgTAT = 0;

  printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
  for(int i = 0; i < n; i++) {
    printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrTime, p[i].burstTime, p[i].compTime, p[i].taTime, p[i].wTime);
    avgWT += p[i].wTime;
    avgTAT += p[i].taTime;
  }

  printf("\nAverage Waiting Time: %.2f", avgWT / n);
  printf("\nAverage Turnaround Time: %.2f\n", avgTAT / n);
}

void roundRobin(Process p[], int n, int quantum) {
  int queue[MAX], front = 0, rear = 0;
  int visited[MAX] = {0}, completed = 0, currentTime = 0;

  // processes arriving at time 0
  for(int i = 0; i < n; i++) {
    if(p[i].arrTime == 0) {
      queue[rear++] = i;
      visited[i] = 1;
    }
  }

  while(completed != n) {
    // If queue is empty, CPU is idle
    if (front == rear) {
      currentTime++;
      for(int i=0; i<n; i++) {
        if(!visited[i] && p[i].arrTime <= currentTime) {
          queue[rear++] = i;
          visited[i] = 1;
        }
      }
      continue;
    }

    // remove process from queue
    int index = queue[front++];
    if(p[index].remTime > quantum) {
      currentTime += quantum;
      p[index].remTime -= quantum;
    } else {
      currentTime += p[index].remTime;
      p[index].remTime = 0;
      p[index].compTime = currentTime;
      completed++;
    }

    // add newly arrived processes
    for(int i = 0; i < n; i++) {
      if(!visited[i] && p[i].arrTime <= currentTime) {
        queue[rear++] = i;
        visited[i] = 1;
      }
    }

    // add unfinished process back to queue
    if(p[index].remTime > 0) {
      queue[rear++] = index;
    }
  }

  calculateTimes(p, n);
  printf("\n Round Robin Scheduling \n");
  display(p, n);
}