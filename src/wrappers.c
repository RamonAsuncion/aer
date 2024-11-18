#include "wrappers.h"

pid_t WaitForPid(pid_t pid, int *wstatus, int options)
{
  pid_t child_pid;

  if ((child_pid = waitpid(pid, wstatus, options)) == -1) {
    perror("WaitForPid error, program aborts.");
    exit(-1);
  }

  return child_pid;
}

pid_t Fork(void)
{
  int pid;

  if ((pid = fork()) == -1) {
    perror("Fork error, program aborts.");
    exit(-1);
  }

  return pid;
}

int Execvp(const char *file, char *const argv[])
{
  if (execvp(file, argv) == -1) {
    perror("Execvp error");
    exit(-1);
  }

  return 0;
}
