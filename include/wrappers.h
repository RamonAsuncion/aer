#ifndef _wrappers_h_
#define _wrappers_h_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>

/**
 * @brief Wrapper for wait()
 * @param wstatus status of child
 * @return pid_t pid of child if successful, -1 otherwise
 */
pid_t Wait(int *wstatus);

/**
 * @brief Wrapper for fork()
 * @return pid_t 0 if child, pid of child if parent, -1 if error
 */
pid_t Fork(void);

/**
 * @brief Wrapper for execvp()
 * @param file path of file to execute
 * @param argv arguments to pass to file
 * @return int 0 if successful, -1 otherwise
 */
int Execvp(const char *file, char *const argv[]);

#endif /* _wrappers_h_ */
