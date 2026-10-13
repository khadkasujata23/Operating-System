#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  pid_t pid;

  // New child process
  pid = fork();

  if(pid < 0) {
    // fork() failed
    printf("Failed to create child process.\n");
  } else if (pid == 0) {
    // block executed by the child process
    printf("\nChild Process\n");
    printf("Child PID : %d\n", getpid());
    printf("Parent PID : %d\n", getppid());
  } else {
    // block executed by the parent process
    printf("\nParent Process\n");
    printf("Parent PID : %d\n", getppid());
    printf("Child PID : %d\n", getpid());
  }
  return 0;
}

/*
  Before fork():
      Process : pid = fork()
  After fork():
    Parent Process: Continues
    Child Process: Continues

The operating system creates a copy of the current process.
After that point, both the parent and the child continue executing from the line
immediately after fork()
*/