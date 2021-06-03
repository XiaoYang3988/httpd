#ifndef __wrap_unix
#define __wrap_unix

#include    "log.h"

#include    <signal.h>
#include    <unistd.h>
#include	<fcntl.h>
#include	<sys/time.h>
#include	<sys/ioctl.h>
#include	<sys/wait.h>
#include	<sys/param.h>

typedef void handler_t(int32_t);
			/* prototypes for our Unix wrapper functions: see {Sec errors} */

void	*Calloc(size_t, size_t);
void	 Close(int32_t);
void	 Dup2(int32_t, int32_t);
int32_t		 Fcntl(int32_t, int32_t, int32_t);
void	 Gettimeofday(struct timeval *, void *);
int32_t		 Ioctl(int32_t, int32_t, void *);
pid_t	 Fork(void);
void	*Malloc(size_t);
int32_t	 Mkstemp(int8_t *);
void	*Mmap(void *, size_t, int32_t, int32_t, int32_t, off_t);
int32_t		 Open(const int8_t *, int32_t, mode_t);
void	 Pipe(int32_t *fds);
ssize_t	 Read(int32_t, void *, size_t);
void	 Sigaddset(sigset_t *, int32_t);
void	 Sigdelset(sigset_t *, int32_t);
void	 Sigemptyset(sigset_t *);
void	 Sigfillset(sigset_t *);
int32_t		 Sigismember(const sigset_t *, int32_t);
void	 Sigpending(sigset_t *);
void	 Sigprocmask(int32_t, const sigset_t *, sigset_t *);
int8_t	*Strdup(const int8_t *);
long	 Sysconf(int32_t);
void	 Sysctl(int32_t *, u_int32_t, void *, size_t *, void *, size_t);
void	 Unlink(const int8_t *);
pid_t	 Wait(int32_t *);
pid_t	 Waitpid(pid_t, int32_t *, int32_t);
void	 Write(int32_t, void *, size_t);
void 	*Realloc(void* ptr, size_t size);
void	 Execve(const char *filename, char *const argv[], char *const envp[]);
handler_t	*Signal(int signum, handler_t *handler);


#endif