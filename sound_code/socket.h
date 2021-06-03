#ifndef __socket
#define __socket

#include    "log.h"
#include    "unpv_config.h"

#include	<sys/socket.h>
#include	<netinet/in.h>
#include    <stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<inttypes.h>
#include 	<sys/select.h>
#ifdef	HAVE_POLL_H
#include	<poll.h>		/* for convenience */
#endif


/* Following shortens all the typecasts of point32_ter arguments: */
#define	SA	struct sockaddr

			/* prototypes for our socket wrapper functions: see {Sec errors} */
int32_t		 Accept(int32_t, SA *, socklen_t *);
void	 Bind(int32_t, const SA *, socklen_t);
void	 Connect(int32_t, const SA *, socklen_t);
void	 Getpeername(int32_t, SA *, socklen_t *);
void	 Getsockname(int32_t, SA *, socklen_t *);
void	 Getsockopt(int32_t, int32_t, int32_t, void *, socklen_t *);
#ifdef	HAVE_INET6_RTH_INIT
int32_t		 Inet6_rth_space(int32_t, int32_t);
void	*Inet6_rth_init(void *, socklen_t, int32_t, int32_t);
void	 Inet6_rth_add(void *, const struct in6_addr *);
void	 Inet6_rth_reverse(const void *, void *);
int32_t		 Inet6_rth_segments(const void *);
struct in6_addr *Inet6_rth_getaddr(const void *, int32_t);
#endif
#ifdef	HAVE_KQUEUE
int32_t		 Kqueue(void);
int32_t		 Kevent(int32_t, const struct kevent *, int32_t,
				struct kevent *, int32_t, const struct timespec *);
#endif
void	 Listen(int32_t, int32_t);
#ifdef	HAVE_POLL
int32_t		 Poll(struct pollfd *, unsigned long, int32_t);
#endif
ssize_t	 Readline(int32_t, void *, size_t);
ssize_t	 Readn(int32_t, void *, size_t);
ssize_t	 Recv(int32_t, void *, size_t, int32_t);
ssize_t	 Recvfrom(int32_t, void *, size_t, int32_t, SA *, socklen_t *);
ssize_t	 Recvmsg(int32_t, struct msghdr *, int32_t);
int32_t		 Select(int32_t, fd_set *, fd_set *, fd_set *, struct timeval *);
void	 Send(int32_t, const void *, size_t, int32_t);
void	 Sendto(int32_t, const void *, size_t, int32_t, const SA *, socklen_t);
void	 Sendmsg(int32_t, const struct msghdr *, int32_t);
void	 Setsockopt(int32_t, int32_t, int32_t, const void *, socklen_t);
void	 Shutdown(int32_t, int32_t);
int32_t		 Sockatmark(int32_t);
int32_t		 Socket(int32_t, int32_t, int32_t);
void	 Socketpair(int32_t, int32_t, int32_t, int32_t *);
void	 Writen(int32_t, void *, size_t);

#endif