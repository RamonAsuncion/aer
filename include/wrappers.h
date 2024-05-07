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
 * @brief Wrapper for pipe()
 * @param pipefd pipefd[0] is for reading, pipefd[1] is for writing
 * @return int 0 if successful, -1 otherwise
 */
int Pipe(int pipefd[2]);

/**
 * @brief Wrapper for fork()
 * @return pid_t 0 if child, pid of child if parent, -1 if error
 */
pid_t Fork(void);

/**
 * @brief Wrapper for read()
 * @param fd file descriptor
 * @param buf buffer to read into
 * @param count number of bytes to read
 * @return int number of bytes read if successful, -1 otherwise
 */
int Read(int fd, void *buf, size_t count);

/**
 * @brief Wrapper for write()
 * @param fd file descriptor
 * @param buf buffer to write from
 * @param count number of bytes to write
 * @return int number of bytes written if successful, -1 otherwise
 */
int Write(int fd, const void *buf, size_t count);

/**
 * @brief Wrapper for wait()
 * @param wstatus status of child
 * @return pid_t pid of child if successful, -1 otherwise
 */
pid_t Wait(int *wstatus);

/**
 * @brief Wrapper for waitpid()
 * @param pid pid of child
 * @param wstatus status of child
 * @param options options to pass to waitpid()
 * @return pid_t pid of child if successful, -1 otherwise
 */
pid_t Waitpid(pid_t pid, int *wstatus, int options);

/**
 * @brief Wrapper for open()
 * @param pathname path of file to open
 * @param flags flags to pass to open()
 * @param mode mode to pass to open()
 * @return int file descriptor if successful, -1 otherwise
 */
int Open(const char *pathname, int flags, mode_t mode);

/**
 * @brief Wrapper for close()
 * @param fd file descriptor
 * @return int 0 if successful, -1 otherwise
 */
int Close(int fd);

/**
 * @brief Wrapper for connect()
 * @param sockfd socket file descriptor
 * @param addr address of socket
 * @param addrlen length of address
 * @return int 0 if successful, -1 otherwise
 */
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief Wrapper for bind()
 * @param sockfd socket file descriptor
 * @param addr address of socket
 * @param addrlen length of address
 * @return int 0 if successful, -1 otherwise
 */
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brif Wrapper for accept()
 * @param sockfd socket file descriptor
 * @param addr address of socket
 * @param addrlen length of address
 * @return int file descriptor of socket if successful, -1 otherwise
 */
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @brief Wrapper for listen()
 * @param sockfd socket file descriptor
 * @param backlog backlog of socket
 * @return int 0 if successful, -1 otherwise
 */
int Listen(int sockfd, int backlog);


/**
 * @brief Wrapper for execvp()
 * @param file path of file to execute
 * @param argv arguments to pass to file
 * @return int 0 if successful, -1 otherwise
 */
int Execvp(const char *file, char *const argv[]);

/**
 * @brief Wrapper for fgets()
 * @param s string to read into
 * @param size number of bytes to read
 * @param stream file stream to read from
 * @return char* string if successful, NULL otherwise
 */
char *Fgets(char *s, int size, FILE *stream);

/**
 * @brief Wrapper for getline()
 * @param lineptr pointer to line to read into
 * @param n number of bytes to read
 * @param stream file stream to read from
 * @return ssize_t number of bytes read if successful, -1 otherwise
 */
ssize_t Getline(char **lineptr, size_t *n, FILE *stream);

#endif /* _wrappers_h_ */
