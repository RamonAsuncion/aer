#include "wrappers.h"

pid_t Wait(int *wstatus)
{
  int pid;
  if ((pid = wait(wstatus)) == -1) {
    perror("Wait error, program aborts.");
    exit(-1);
  }
  return pid;
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
  execvp(file, argv);
  perror("Execvp error, program aborts.");
  exit(-1);
}

