#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  int i;
  pid_t pid;

  // Creating 3 child processes
  for(i=1; i<=3; i++) {
    pid = fork();
    if(pid == 0) {
      // child process
      printf("Child %d: PID = %d, Parent PId = %d\n", i, getpid(), getppid());
      return 0; // child exists so it doesn't create more children
    }
  }

  // Parent Process
  printf("Parent Process: PID = %d\n", getpid());
  return 0;
}