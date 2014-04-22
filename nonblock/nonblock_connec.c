#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
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
int connect_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = connect(sockfd, addr, addrlen)) < 0 && errno != EINPROGRESS)
	{
		fprintf(stdout, "connect failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return ret;
}
int inet_pton_func(int af, const char *src, void *dst)
{
	int ret;
	if ((ret = inet_pton(af, src, dst)) < 0)
	{
		fprintf(stdout, "does not contain a valid address family\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "invalid ip address\n");
		exit(EXIT_FAILURE);
	}
	return ret;
}
int fcntl_func(int fd, int cmd, int flag)
{
	int ret;
	if ((ret = fcntl(fd, cmd, flag)) < 0)
	{
		fprintf(stdout, "fcntl error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
void nonblock(int fd)
{
	int opt;
	opt = fcntl_func(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, opt | O_NONBLOCK);
	return;
}
int main(int argc, char *argv[])
{
	int fd;
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	nonblock(fd);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	int ret;
	ret = connect_func(fd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret == 0)
		goto done;
#if 1
	if (errno == EINPROGRESS)
		fprintf(stdout, "is EINPROGRESS\n");
#endif
	usleep(1000);
	fprintf(stdout, "usleep(1000)\n");
	
	fd_set rset;
	fd_set wset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(fd, &rset);
	FD_SET(fd, &wset);
	ret = select(fd + 1, &rset, &wset, NULL, NULL);
	int error = 0;
	int len = sizeof(error);
	if (FD_ISSET(fd, &rset) || FD_ISSET(fd, &wset))
		getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);
done:
	if (error)
	{
		close(fd);
		fd = -1;
		errno = error;
	}
	send(fd, "111",sizeof("111"), 0);

	return 0;
}
