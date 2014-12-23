#ifndef __SHARE_H__
#define __SHARE_H__
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int bind_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = bind(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int inet_pton_func(int af, const char *src, void *dst)
{
	int ret;
	if ((ret = inet_pton(af, src, dst)) < 0)
	{
		fprintf(stdout, "ip address family failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "ip address set error, please reset\n");
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t recvfrom_func(int sockfd, void *buf, size_t len, int flags,
		struct sockaddr *src_addr, socklen_t *addrlen)
{
	int n;
	if ((n = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0)
	{
		fprintf(stdout, "recvfrom error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return n;
}
int connect_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int fd;
	if ((fd = connect(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}

ssize_t send_func(int sockfd, const void *buf, size_t len, int flags)
{
	int n;
	if ((n = send(sockfd, buf, len, flags)) < 0)
	{
		fprintf(stdout, "send error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return n;
}
int accept_func(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int fd;
	if ((fd = accept(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stdout, "accept error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
#include <signal.h>
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal_func(int signum, sighandler_t handler)
{
	sighandler_t  sigfp = NULL;
	if ((sigfp = signal(signum, handler)) == SIG_ERR)
	{
		fprintf(stdout, "signal error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (sigfp == SIG_DFL)
	{
		fprintf(stdout, "instead default signal function\n");
	}
	return sigfp ;
}
ssize_t recv_func(int sockfd, void *buf, size_t len, int flags)
{
	int n;
	if ((n = recv(sockfd, buf, len, flags)) < 0)
	{
		fprintf(stdout, "recv error :%s\n", strerror(errno));
		if (errno == EAGAIN)
			fprintf(stdout, "errno again :%d\n", errno);
		exit(EXIT_FAILURE);
	}
	return n;
}
#include <unistd.h>
#include <fcntl.h>
int fcntl_func(int fd, int mod, int arg)
{
	int ret;
	if ((ret = fcntl(fd, mod, arg)) < 0)
	{
		fprintf(stdout, "fcntl error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int select_func(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout)
{
	int ret;
	if ((ret = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
	{
		fprintf(stdout, "select error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int accept4_func(int sockfd, struct sockaddr *addr,
	  socklen_t *addrlen, int flags)
{
	int confd;
	if ((confd = accept4(sockfd, addr, addrlen, flags)) < 0)
	{
		fprintf(stdout, "accept4 error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return confd;
}
int setsockopt_func(int sockfd, int level, int optname,
	      const void *optval, socklen_t optlen)
{
	int ret;
	if ((ret = setsockopt(sockfd, level, optname, optval, optlen)) < 0)
	{
		fprintf(stdout, "setsockopt error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int listen_func(int sockfd, int backlog)
{
	int ret;
	if ((ret = listen(sockfd, backlog)) < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
#endif
