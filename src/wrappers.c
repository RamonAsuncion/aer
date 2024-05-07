#include "wrappers.h"

pid_t Wait(int *wstatus)
{
  int pid;
  if ((pid = wait(wstatus)) == -1) {
    perror("Wait error, program aborts.");
    exit(-1);
  }
  return pid;
}

pid_t Waitpid(pid_t pid, int *wstatus, int options)
{
  int result;
  if ((result = waitpid(pid, wstatus, options)) == -1) {
    perror("Waitpid error, program aborts.");
    exit(-1);
  }
  return result;
}

int Open(const char *pathname, int flags, mode_t mode)
{
  int fd;
  if ((fd = open(pathname, flags, mode)) == -1) {
    perror("Open error, program aborts.");
    exit(-1);
  }
  return fd;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int result;
  if ((result = connect(sockfd, addr, addrlen)) == -1) {
    perror("Connect error, program aborts.");
    exit(-1);
  }
  return result;
}

int Socket(int domain, int type, int protocol)
{
  int fd;
  if ((fd = socket(domain, type, protocol)) == -1) {
    perror("Socket error, program aborts.");
    exit(-1);
  }
  return fd;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int result;
  if ((result = bind(sockfd, addr, addrlen)) == -1) {
    perror("Bind error, program aborts.");
    exit(-1);
  }
  return result;
}

int Listen(int sockfd, int backlog)
{
  int result;
  if ((result = listen(sockfd, backlog)) == -1) {
    perror("Listen error, program aborts.");
    exit(-1);
  }
  return result;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
  int fd;
  if ((fd = accept(sockfd, addr, addrlen)) == -1) {
    perror("Accept error, program aborts.");
    exit(-1);
  }
  return fd;
}

int Close(int fd)
{
  int result;
  if ((result = close(fd)) == -1) {
    perror("Close error, program aborts.");
    exit(-1);
  }
  return result;
}

int Read(int fd, void *buf, size_t count)
{
  int result;
  result = read(fd, buf, count);
  if (result < 0) {
    perror("Read error, program aborts.");
    exit(-1);
  }
  return result;
}

int Write(int fd, const void *buf, size_t count) {
  int result;
  result = write(fd, buf, count);
  if (result < 0) {
    perror("Write error, program aborts.");
    exit(-1);
  }
  return result;
}

int Pipe(int pipefd[])
{
  int result;
  if ((result = pipe(pipefd)) == -1) {
    perror("Pipe error, program aborts.");
    exit(-1);
  }
  return result;
}

pid_t Fork(void)
{
  int pid;
  if ((pid = fork()) == -1) {
    perror("Fork error, program aborts.");
    exit(-1);
  }
  return pid;
}

// execvp
int Execvp(const char *file, char *const argv[])
{
  execvp(file, argv);
  perror("Execvp error, program aborts.");
  exit(-1);
}

char *Fgets(char *s, int size, FILE *stream)
{
  char *result = fgets(s, size, stream);
  if (result == NULL) {
    if (feof(stream))
      return NULL;
    else {
      perror("Fgets error, program aborts.");
      exit(-1);
    }
  }
  return result;
}

ssize_t Getline(char **lineptr, size_t *n, FILE *stream)
{
  ssize_t result = getline(lineptr, n, stream);
  if (result == -1) {
    if (feof(stream))
      return -1;
    else {
      perror("Getline error, program aborts.");
      exit(-1);
    }
  }
  return result;
}

