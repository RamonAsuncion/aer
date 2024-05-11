#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "wrappers.h"

void execute_command(char *command)
{

  char *arg = strtok(command, " \n");
  char **args = malloc(sizeof(char*));
  int i = 0;
  while (arg != NULL) {
    args[i] = arg;
    arg = strtok(NULL, " \n");
    args = realloc(args, (i+2) * sizeof(char*));
    i++;
  }
  args[i] = NULL;

  pid_t pid = Fork();
  if (pid == 0) {
    Execvp(args[0], args);
  } else {
    int status;
    Wait(&status);
    if (WEXITSTATUS(status) == 0) {
      printf("[ishell: program terminated successfully]\n");
    } else {
      printf("[ishell: program terminated abnormally %d]\n", WEXITSTATUS(status));
    }
  }
}


int main(int argc, char *argv[])
{
  // Bind the tab key to the rl_complete function
  rl_bind_key('\t', rl_complete);

  // Clear the screen with Ctrl + L
  rl_bind_keyseq("\\C-l", rl_clear_screen);

  // Get the home directory
  char *home = getenv("HOME");

  // Initialize the history list
  char history_path[1024];
  sprintf(history_path, "%s/.history", home);
  read_history(history_path);

  while (true) {
    char *command = readline("ishell> ");
    add_history(command);

    if (strcmp(command, "exit") == 0) break;

    if (strchr(command, ';') != NULL) {
      char *cmd = strtok(command, ";");
      char **cmds = malloc(sizeof(char*));
      int i = 0;
      while (cmd != NULL) {
        cmds[i] = cmd;
        cmd = strtok(NULL, ";");
        cmds = realloc(cmds, (i+2) * sizeof(char*));
        i++;
      }
      cmds[i] = NULL;

      for (int j = 0; j < i; ++j) {
        execute_command(cmds[j]);
      }
    } else {
      execute_command(command);
    }

    free(command);
  }

  return 0;
}

