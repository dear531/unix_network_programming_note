#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

ssize_t sendto_func(int sockfd, const void *buf, size_t len, int flags,
	      const struct sockaddr *dest_addr, socklen_t addrlen)
{
	int n;
	if ((n = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0)
	{
		fprintf(stdout, "sendto failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return n;
}

int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
	{
		fprintf(stdout, "socket failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int inet_pton_func(int af, const char *src, void *dst)
{
	int ret;
	if ((ret = inet_pton(af, src, dst)) < 0)
	{
		fprintf(stdout, "address family error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "ip address format error\n");
		exit(EXIT_FAILURE);
	}
	return ret;
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

int main(int argc, char *argv[])
{
	int fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int opt = 1;
	setsockopt_func(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	struct sockaddr_in broadaddr;	
	broadaddr.sin_family	= AF_INET;
	broadaddr.sin_port	= htons(8000);
	inet_pton_func(AF_INET, argv[1], &broadaddr.sin_addr);
	char buf[1024];
	int n;
	for ( ; ; )
	{
		sendto_func(fd, "hello\n", strlen("hello\n"), 0, (struct sockaddr*)&broadaddr, sizeof(broadaddr));
#if 0
		bzero(buf, sizeof(buf));
		n = recv(fd, buf, sizeof(buf), 0);
		write(STDOUT_FILENO, buf, n);
#else
		sleep(1);
		fprintf(stdout, "sendto %s hello\n", argv[1]);
#endif
	}

	return 0;
}
