#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define pcinfo()\
	fprintf(stdout, "func:%s file:%s line:%d date:%s time:%s\n",\
			__func__, __FILE__, __LINE__, __DATE__, __TIME__);

inline void peinfo(char *p)
{
	fprintf(stdout, "%s error:%s\n", p, strerror(errno));
}

#define painfo(prampt)		\
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
int func_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = connect(sockfd, addr, addrlen)) < 0)
	{
		painfo("connect");
		exit(EXIT_FAILURE);
	}
	return ret;
}
char *func_gets(char *s)
{
	char *p;
	if ((p = gets(s)) == NULL)
	{
		painfo("gets");
	}
	return p;
}

ssize_t func_send(int sockfd, const void *buf, size_t len, int flags)
{

	ssize_t n;
	if ((n = send(sockfd, buf, len, flags)) < 0)
	{
		painfo("send");
		exit(EXIT_FAILURE);
	}
	return n;
}
ssize_t func_recv(int sockfd, void *buf, size_t len, int flags)
{
	ssize_t n;
	if ((n = recv(sockfd, buf, len, flags)) < 0)
	{
		painfo("recv");
		exit(EXIT_FAILURE);
	}
	return n;
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

	int
main(int argc, char *argv[])
{
	int confd;
	struct sockaddr_in saddr;
	char sbuf[1024] = "12345", cbuf[1024];

	confd = func_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	func_connect(confd, (struct sockaddr *)&saddr, sizeof(saddr));
#if 1

	bzero(sbuf, sizeof(sbuf));
	//	func_gets(sbuf);
	//while(func_gets(sbuf))
	while(scanf("%s", sbuf))
	{
		fprintf(stdout, "sbuf:%s\n", sbuf);
		func_send(confd, sbuf, sizeof(sbuf) > strlen(sbuf) ? strlen(sbuf) : sizeof(sbuf), 0);

		bzero(cbuf, sizeof(cbuf));
		func_recv(confd, cbuf, sizeof(cbuf), 0);
		fprintf(stdout, "cbuf:%s\n", cbuf);
	}
#endif
	func_close(confd);

	exit(0);


	return 0;
}
