#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <libgen.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "wrappers.h"

#define MAX_ARGS     64
#define BUFFER_SIZE  1024
#define HISTORY_SIZE 1024

char *last_working_dir = NULL;

void change_directory(char *new_dir)
{
  char *prev_working_dir = getcwd(NULL, 0);
  if (chdir(new_dir) == -1) {
    perror("ishell: cd");
  } else {
    free(last_working_dir);
    last_working_dir = prev_working_dir;
  }
}

void execute_command(char *command)
{
  char *arg = strtok(command, " \n");
  char **args = malloc(MAX_ARGS * sizeof(char*));
  int i = 0;
  while (arg != NULL) {
    args[i] = arg;
    arg = strtok(NULL, " \n");
    i++;
  }
  args[i] = NULL;

  // Handle 'cd' command.
  if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
      change_directory(getenv("HOME"));
    } else if (strcmp(args[1], ".") == 0) {
      // No action needed, current directory.
    } else if (strcmp(args[1], "..") == 0) {
      char *parent_dir = dirname(getcwd(NULL, 0));
      change_directory(parent_dir);
    } else if (strcmp(args[1], "-") == 0) {
      if (last_working_dir != NULL) {
        change_directory(last_working_dir);
      }
    } else {
      change_directory(args[1]);
    }
    return;
  }

  pid_t pid = Fork();
  if (pid == 0) {
    Execvp(args[0], args);
  } else if (pid > 0) {
    int status;
    Wait(&status);
#if DEBUG
    if (WIFEXITED(status)) {
      printf("[ishell: program terminated successfully]\n");
    } else {
      printf("[ishell: program terminated abnormally %d]\n", WEXITSTATUS(status));
    }
#endif
  } else {
    perror("[ishell: fork failed]");
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
  char history_path[HISTORY_SIZE];
  sprintf(history_path, "%s/.history", home);
  read_history(history_path);

  while (true) {
    char *command = readline("ishell> ");
    if (!command) break;

    add_history(command);

    if (strcmp(command, "exit") == 0) break;

    // Command split with semicolon.
    if (strchr(command, ';') != NULL) {
      char *cmd = strtok(command, ";");
      char **cmds = malloc(sizeof(char*));
      int i = 0;
      while (cmd != NULL) {
        cmds[i] = cmd;
        cmd = strtok(NULL, ";");
        cmds = realloc(cmds, (i + 2) * sizeof(char*));
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

  return EXIT_SUCCESS;
}

