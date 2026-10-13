#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t pid;

  // child process
  pid = fork();

  if(pid < 0) {
    printf("Failed to create child process.\n");
    return 1;
  } else if (pid == 0) {
    printf("Child Process is running.\n");
    printf("Child PID : %d\n", getpid());
    printf("Child Process is completed.\n");
  } else {
    // Parent process waits for the child
    wait(NULL);
    printf("\nParent Process resumes after child finishes.\n");
    printf("Parent PID : %d\n", getppid());
  }
  return 0;
}