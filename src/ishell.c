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

// TODO: Writing the wrong command "sl" then writing exit does not work.
// TODO: alias command

#include "platform.h"
#include "wrappers.h"

#define MAX_ARGS     64
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
  if (!command || !(command[0])) return;

  char *arg = strtok(command, " \n");
  char **args = malloc(MAX_ARGS * sizeof(char*));
  int i = 0;
  while (arg != NULL) {
    args[i] = arg;
    arg = strtok(NULL, " \n");
    i++;
  }
  args[i] = NULL;

  if (strcmp(command, "exit") == 0) {
    printf("Exit!\n");
    exit(0);
  } else if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
      change_directory(getenv("HOME"));
    } else if (strcmp(args[1], ".") == 0) {
      // No action needed, current directory.
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
    //HIST_ENTRY **h = history_list();
    //if (h) {
    //  for (int i = 0; h[i]; ++i) {
    //    printf("%5d  %s\n", i + history_base, h[i]->line);
    //  }
    //}
    //return;
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
    // TODO: a few problems. 1. the flags don't get deleted, the quotes stay, and the new line removed does not work, and implementing escape.
    return;
  }
  // FIXME: This has to be part of the execution for the fork command.
  //else {
  //  fprintf(stderr, "ishell: %s command not found.\n", args[0]);
  //}

  pid_t pid = Fork();
  if (pid == 0) {
    // fixme: Using wrapper function has it's own error ahndling already.
    if (execvp(args[0], args) == -1 && errno == ENOENT) {
      fprintf(stderr, "ishell: %s: command not found.\n", args[0]);
    } else {
      Execvp(args[0], args);
    }
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

int main(int argc, char **argv)
{
  set_key_bindings();

  // Get the home directory
  char *home = getenv("HOME");

  // Initialize the history list
  char history_path[HISTORY_SIZE];
  snprintf(history_path, sizeof(history_path), "%s/.history", home);
  // sprintf(history_path, "%s/.history", home);
  read_history(history_path);

  while (true) {
    char *command = readline("ishell> ");
    if (!command) break;

    // Add the command to history list.
    add_history(command);

    // Write the history to file.
    write_history(history_path);

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

