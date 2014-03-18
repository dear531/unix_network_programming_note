#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>

#include "bit_check.h"

#define pcinfo()\
fprintf(stdout, "func:%s file:%s line:%d date:%s time:%s\n",\
__func__, __FILE__, __LINE__, __DATE__, __TIME__);

inline void peinfo(char *p)
{
	fprintf(stdout, "%s error:%s\n", p, strerror(errno));
}

#define painfo(prampt)	\
do{			\
	pcinfo();	\
	peinfo(prampt);	\
}while(0)

int func_socket(int domain, int type, int protocol)
{
	int connfd;
	if ((connfd = socket(domain, type, protocol)) < 0)
	{
		painfo("socket");
		exit(EXIT_FAILURE);
	}
	return connfd;
}
int func_inet_pton(int af, const char *src, void *dst)
{
	int ret;
	if ((ret = inet_pton(af, src, dst)) == 1)
	{
		return ret;
	}
	else if (ret == 0)
	{
		fprintf(stdout, "%s addr ip invalid form\n", src);
		exit(EXIT_FAILURE);
	}
	else if (ret == -1)
	{
		painfo("address family invalid\n");
		exit(EXIT_FAILURE);
	}
}
int func_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = bind(sockfd, addr, addrlen)) < 0)
	{
		painfo("bind");
		exit(EXIT_FAILURE);
	}
	return ret;
}

int func_listen(int sockfd, int backlog)
{
	int ret;
	if ((ret = listen(sockfd, backlog)) < 0)
	{
		painfo("listen");
		exit(EXIT_FAILURE);
	}
	return ret;
}

int func_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
#if 1
	int fd;
	if ((fd = accept(sockfd, addr, addrlen)) < 0)
	{
		painfo("accept");
		exit(EXIT_FAILURE);
	}
#endif
	return fd;
}

int func_close(int fd)
{
	int ret;
	if ((ret = close(fd)) < 0)
	{
		painfo("close");
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t func_recv(int sockfd, void *buf, size_t len, int flags)
{
	size_t ret;
	if ((ret = recv(sockfd, buf, len, flags)) < 0)
	{
		painfo("recv");
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t func_send(int sockfd, const void *buf, size_t len, int flags)
{
	ssize_t ret;
	if ((ret = send(sockfd, buf, len, flags)) < 0)
	{
		painfo("send");
		exit(EXIT_FAILURE);
	}
	return ret;
}

int
main(int argc, char *argv[])
{
	pid_t pid;
	int listfd, connfd;
	struct sockaddr_in saddr;
	char sbuf[1024], cbuf[1024];
	int n;

	listfd = func_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//func_inet_pton(AF_INET, INADDR_ANY, &saddr.sin_addr);
	func_bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr));

	func_listen(listfd, 100);

	for ( ; ; )
	{
		connfd  = func_accept(listfd, NULL, NULL);
		pid = fork();
		if (pid == 0)
		{
			int a, b;
			/* is child process */
			func_close(listfd);
			bzero(sbuf, sizeof(sbuf));
			while(n = func_recv(connfd, sbuf, sizeof(sbuf), 0))
			{
				sscanf(sbuf, "%d,%d", &a, &b);
				fprintf(stdout, "a:%d,b:%d\n", a, b);

			//bcopy(sbuf, cbuf, n);
			//bzero(&cbuf[n], sizeof(cbuf) - n);
				sprintf(cbuf, "%d", a + b);
			func_send(connfd, cbuf, n, 0);
			}

			func_close(connfd);
			exit(0);
		}
		else if (pid > 0)
		{
			/* is parent process */
			func_close(connfd);
		}
		else
		{
			/* fork error */
			fprintf(stdout, "fork error:%s\n", strerror(errno));
		}
	}
	func_close(listfd);
	return 0;
}
