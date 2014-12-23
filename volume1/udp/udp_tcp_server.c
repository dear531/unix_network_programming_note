#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int socket_func(int domain, int type, int protocol)
{
	int fd;
	fd = socket(domain, type, protocol);
	if (fd < 0)
	{
		fprintf(stdout, "socket error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int bind_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	ret = bind(sockfd, addr, addrlen);
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int listen_func(int sockfd, int backlog)
{
	int ret;
	ret = listen(sockfd, backlog);
	if (ret < 0)
	{
		fprintf(stdout, "listen error :%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}

int accept_func(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int fd;
	fd = accept(sockfd, addr, addrlen);
	if (fd < 0)
	{
		fprintf(stdout, "accept error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int select_func(int nfds, fd_set *readfds, fd_set *writefds,
		   fd_set *exceptfds, struct timeval *timeout)
{
	int ret;
	ret = select(nfds, readfds, writefds, exceptfds, timeout);
	if (ret < 0)
	{
		fprintf(stdout, "select error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}


#define TCPLSTN	0
#define UDPNUM	1
#define FDNUM	20

int main(int argc, char *argv[])
{
	int fds[FDNUM];
	int i;
	for (i = 0; i < FDNUM; i++)
		fds[i] = -1;
#if 1	/* TCP PART */
	/* tcp scoket */
	fds[TCPLSTN] = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set reuseaddr */
	int opt = 1;
	setsockopt(fds[TCPLSTN], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	/* tcp bind */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);	
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind_func(fds[TCPLSTN], (struct sockaddr*)&saddr, sizeof(saddr));
	/* tcp listen */
	listen_func(fds[TCPLSTN], 10);
	char tsbuf[1024];
	int confd;
#endif
	/* udp socket */
	struct sockaddr_in ludpaddr, ucaddr;
	int ucl = sizeof(ucaddr);
	char usbuf[1024];
	fds[UDPNUM] = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	/* set udp reuseaddr */
	int uopt = 1;
	setsockopt(fds[UDPNUM], SOL_SOCKET, SO_REUSEADDR, &uopt, sizeof(uopt));
	/* udp bind */
	bzero(&ludpaddr, sizeof(ludpaddr));
	ludpaddr.sin_family = AF_INET;
	ludpaddr.sin_port = htons(8000);	
	inet_pton(AF_INET, "127.0.0.1", &ludpaddr.sin_addr);
	bind_func(fds[UDPNUM], (struct sockaddr*)&ludpaddr, sizeof(ludpaddr));
	bzero(usbuf, sizeof(usbuf));
#if 1	/* multiplex I/O */
	int maxi, maxfd;
	maxi = TCPLSTN > UDPNUM ? TCPLSTN : UDPNUM;
	maxfd = fds[TCPLSTN] > fds[UDPNUM] ? fds[TCPLSTN] : fds[UDPNUM];
	fd_set rset;
	fd_set wset;
	int num;
	int j;
	int n;
	
	for ( ; ; )
	{
		sleep(1);
		for (i = 0; i < FDNUM; i++)
			fprintf(stdout, "fds[%d]:%d\n", i, fds[i]);
		putchar('\n');
		fprintf(stdout, "maxi :%d, maxfd :%d\n", maxi, maxfd);
		/* select */
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		for (i = 0; i < FDNUM; i++)
		{
			if (fds[i] != -1)
			{
				FD_SET(fds[i], &rset);
				fprintf(stdout, "set fds[%d]:%d\n", i, fds[i]);
			}
		}

		num = select_func(maxfd + 1, &rset, NULL, NULL, NULL);
		for (i = 0; i <= maxi; i++)
		{
			if (!FD_ISSET(fds[i], &rset))
				continue;
			if (i == TCPLSTN)
			{
			/* tcp accept */
				for (j = 0; j < FDNUM; j++)
				{
					if (fds[j] == -1)
						break;
				}
				fds[j] = accept_func(fds[TCPLSTN], NULL, NULL);
				if (maxi < j)
					maxi = j;
				if (maxfd < fds[j])
					maxfd = fds[j];
			}
			else if (i == UDPNUM)
			{
			/* udp client */
				bzero(&ucaddr, sizeof(ucaddr));
				bzero(usbuf, sizeof(usbuf));
				recvfrom(fds[UDPNUM], usbuf, sizeof(usbuf), 0, (struct sockaddr*)&ucaddr, &ucl);
				sendto(fds[UDPNUM], usbuf, strlen(usbuf), 0, (struct sockaddr*)&ucaddr, ucl);
			}
			else
			{
			/* tcp client */
				bzero(tsbuf, sizeof(tsbuf));
				n = recv(fds[i], tsbuf, sizeof(tsbuf), 0);
				if (n == 0)
				{
					close(fds[i]);
					fds[i] = -1;
					while (fds[maxi] == -1)
						maxi--;
					if (maxfd != fds[maxi])
						maxfd = fds[maxi];
					continue;
				}
				send(fds[i], tsbuf, strlen(tsbuf), 0);
			}
			if (--num <= 0)
				break;
		}
	}
#endif
	/* tcpclose */
	/* udp close */
	for (i = 0; i < FDNUM; i++)
	{
		if (fds[i] != -1)
		{
			close(fds[i]);
			fds[i] = -1;
		}
	}

		return 0;
}
