#include <stdio.h>
#include <unistd.h>

int main() {
  // getpid() returns the Process ID (PID) of the currently running process.
  printf("Current Process Id (PID): %d \n", getpid());
  // getppid() returns the Parent Process ID (PPID), which is the PID of the process that created the current process (usually the shell or another parent process).
  printf("Parent Process Id (PPID): %d \n", getppid());
  return 0;
}