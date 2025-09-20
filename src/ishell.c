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

// TODO: Add ability to change "PS1"?
// TODO: Make text a certain color.

#define MAX_ARGS     64
#define HISTORY_SIZE 1024
#define MAX_ALIAS    10
#define PATH_MAX     4096

struct aliases {
  char *name;
  char *command;
};

struct aliases alias[MAX_ALIAS];

char *last_working_dir = NULL;
int alias_count = 0;

void add_alias(char *name, char *command)
{
  if (alias_count < MAX_ALIAS) {
    alias[alias_count].name = strdup(name);
    alias[alias_count].command = strdup(command);
    alias_count++;
  }
}

char *get_alias_command(char *name)
{
  // TODO: probably use hash table
  for (int i = 0; i < alias_count; ++i) {
    if (strcmp(alias[i].name, name) == 0)
      return alias[i].command;
  }
  return NULL;
}

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

char **parse_args(char *cmd)
{
  char *token = strtok(cmd, " \n");
  char **args = malloc(MAX_ARGS * sizeof(char*));
  int i = 0;

  while (token != NULL) {
    args[i] = token;
    token = strtok(NULL, " \n");
    i++;
  }
  args[i] = NULL;
  return args;
}

char **split_string(char *str, char *delim)
{
  char *str_cpy = strdup(str);
  if (str_cpy == NULL) return NULL;

  int num_tokens = 0;
  char *token = strtok(str_cpy, delim);
  while (token != NULL) {
    num_tokens++;
    token = strtok(NULL, delim);
    printf("token: %s\n", token);
  }
  printf("total tokens: %d\n", num_tokens);
  free(str_cpy);

  char **res = malloc((num_tokens + 1) * sizeof(char*));
  if (res == NULL) return NULL;

  free(res);

  return NULL;
}

void execute_command(char *command)
{
  if (!command || !(command[0])) return;

  char path[PATH_MAX];
  pid_t pid;
  int status;

  char **args = parse_args(command);

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
      if (last_working_dir != NULL)
        change_directory(last_working_dir);
    } else {
      change_directory(args[1]);
    }
    return;
  } else if (strcmp(args[0], "history") == 0) {
    for (int i = history_base; i < history_length; i++) {
      HIST_ENTRY *entry = history_get(i);
      if (entry)
        printf("%5d  %s\n", i + 1, entry->line);
    }
    return;
  } else if (strcmp(args[0], "echo") == 0) {
    for (int i = 1; args[i] != NULL; i++)
      printf("%s ", args[i]);
    printf("\n");
    return;
  } else if (strcmp(args[0], "alias") == 0) {
    // TODO: Why is it args[2]?
    // TODO: When the command is alias it should stop trying to use parse_args and use the
    // Waste of computation just straight up using command
    // split string function.
    printf("command: %s\n", command);
    return;
  } else if (strcmp(args[0], "pwd") == 0) {
    memset(path, 0, sizeof(path));
    if (getcwd(path, sizeof(path)) == NULL) {
      perror("getcwd");
      exit(EXIT_FAILURE);
    }
    printf("%s\n", path);
    return;
  }

  pid = Fork();
  if (pid == 0) {
    // Child
    if (execvp(args[0], args) == -1 && errno == ENOENT)
      fprintf(stderr, "ishell: %s: command not found.\n", args[0]);
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

  const char *ps1 = "ishell> ";

  while (true) {
    char *command = readline(ps1);
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

      for (int j = 0; j < i; ++j)
        execute_command(cmds[j]);
    } else {
      execute_command(command);
    }

    free(command);
  }

  return EXIT_SUCCESS;
}
