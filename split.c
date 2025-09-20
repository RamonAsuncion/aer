#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **split_string(char *str, const char *delim)
{
  char *str_copy = strdup(str);
  if (str_copy == NULL) return NULL;

  int num_tokens = 0;
  char *token = strtok(str_copy, delim);
  while (token != NULL) {
    num_tokens++;
    token = strtok(NULL, delim);
  }
  free(str_copy);

  char **res = malloc((num_tokens + 1) * sizeof(char*));
  if (res == NULL) return NULL;

  str_copy = strdup(str);
  if (str_copy == NULL) {
    free(res);
    return NULL;
  }

  token = strtok(str_copy, delim);
  int i = 0;
  while (token != NULL) {
    res[i] = strdup(token);
    if (res[i] == NULL) {
      for (int j = 0; j < i; ++j)
        free(res[j]);
      free(res);
      free(str_copy);
      return NULL;
    }
    i++;
    token = strtok(NULL, delim);
  }
  res[num_tokens] = NULL;
  free(str_copy);
  return res;
}

int main()
{
  char *cmd = "alias";
  char str[] = "ls=\"ls -la\"";
  char **parts = split_string(str, "=");

  if (parts != NULL) {
    printf("Command: %s\n", cmd);
    printf("Key:     %s\n", parts[0]);
    printf("Value:   %s\n", parts[1]);

    for (int i = 0; parts[i] != NULL; ++i)
      free(parts[i]);
    free(parts);
  }
  return 0;
}
