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

#define MULTI_CAST_PORT	8000
#define MULIT_CAST_IP	"224.0.0.8"

#endif
