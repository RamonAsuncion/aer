#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

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
 * @param pid set of child processes for which to wait
 * @param wstatus status of child
 * @param options options provided to the waitpid function
 * @return pid_t pid of child if successful, -1 otherwise
 */
pid_t WaitForPid(pid_t pid, int *wstatus, int options);

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

#endif /* _WRAPPERS_H_ */
