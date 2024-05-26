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


//  cd: Changes the current working directory.
//  echo: Displays a line of text or variable values.
//  exit: Terminates the shell process. (DONE)
//  export: Sets environment variables.
//  alias: Creates a shortcut (alias) for a command.
//  history: Displays a list of previously executed commands.
//  pwd: Prints the current working directory.
//  source or .: Executes commands from a file in the current shell context.
//  unset: Removes the value of a variable or variables.

char* last_working_dir = NULL;

// void handle_cd_command

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

  // FIXME: switch statement to handle the commands?

  // Handle 'cd' command.
  if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
      printf("Go home.\n");
      chdir(getenv("HOME"));
    } else if (strcmp(args[1], ".") == 0) {
      printf ("Current directory: .\n");
      return;
    } else if (strcmp(args[1], "..") == 0) {
      printf("Go back a directory: ..\n");
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        chdir(cwd);
      }
    } else if (strcmp(args[1], "-") == 0) {
      printf("Previous directory: ..\n");
      printf("-\n");
      if (last_working_dir != NULL) {
        chdir(last_working_dir);
      }
    } else {
      if (chdir(args[1]) != 0) {
        perror("ishell: cd");
      }
    }
    return;
  }

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

