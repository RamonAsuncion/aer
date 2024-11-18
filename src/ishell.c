#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <libgen.h>
#include <errno.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "platform.h"
#include "wrappers.h"

#define MAX_ARGS     64
#define HISTORY_SIZE 1024


char *last_working_dir = NULL;

/**
 * Changes the current working directory and updates the last working directory.
 */
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

/**
 * Parses and executes the given command.
 */
void execute_command(char *command)
{
  if (!command || !(command[0])) return;

  char *arg = strtok(command, " \n");
  char **args = malloc(MAX_ARGS * sizeof(char*));
  pid_t pid;
  int status;
  int i = 0;

  while (arg != NULL) {
    args[i] = arg;
    arg = strtok(NULL, " \n");
    i++;
  }
  args[i] = NULL;

  if (strcmp(command, "exit") == 0) {
    exit(0);
  } else if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
      change_directory(getenv("HOME"));
    } else if (strcmp(args[1], ".") == 0) {
      return;
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
  } else if (strcmp(args[0], "history") == 0) {
    for (int i = history_base; i < history_length; i++) {
      HIST_ENTRY *entry = history_get(i);
      if (entry) {
        printf("%5d  %s\n", i + 1, entry->line);
      }
    }
    return;
  } else if (strcmp(args[0], "echo") == 0) {
    for (int i = 1; args[i] != NULL; i++) {
      printf("%s ", args[i]);
    }
    printf("\n");
    return;
  }

  pid = Fork();
  if (pid == 0) {
    // Child
    if (execvp(args[0], args) == -1 && errno == ENOENT) {
      fprintf(stderr, "ishell: %s: command not found.\n", args[0]);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("[ishell: fork failed]");
  } else {
    // Parent
    do {
      WaitForPid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
}

/**
 * Main shell loop that handles user input and command execution.
 */
int main(int argc, char **argv)
{
  set_key_bindings();

  char *home = getenv("HOME");

  char history_path[HISTORY_SIZE];
  snprintf(history_path, sizeof(history_path), "%s/.history", home);
  read_history(history_path);

  while (true) {
    char *command = readline("ishell> ");
    if (!command) break;

    add_history(command);
    write_history(history_path);

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
