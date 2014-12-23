#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

int setnonblock(int fd)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL)) < 0)
	{
		fprintf(stdout, "get fd status failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		fprintf(stdout, "set fd status failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
int create_server()
{
#if 1
	/* scoket */
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		fprintf(stdout, "socket error :%s\n",  strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* set address reuse */
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		fprintf(stdout, "warning: set address reuse failed\n");
	/* bind */
	int ret;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	if ((ret = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr)) < 0)
	{
		fprintf(stdout, "address family error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "address format error\n");
		exit(EXIT_FAILURE);
	}
	if (bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
	if (listen(fd, 10) < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
#endif
}
int create_client()
{
	int fd;
#if 1
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	int ret;
	if ((ret = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr)) < 0)
	{
		fprintf(stdout, "address family error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "address format error\n");
		exit(EXIT_FAILURE);
	}
	if ((ret = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr))) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
#else
	/* O_NONBLOCK at fcntl invalid for normal diveci file */
	fd = open("1.txt", O_CREAT | O_TRUNC | O_RDWR, 0755);
#endif
	return fd;
}
int rdwt_once(int fd)
{
	int count = 0;
	char buf[1024];
	int n;
	bzero(buf, sizeof(buf));
tryagain:
	if ((n = read(fd, buf, sizeof(buf))) < 0)
	{
		if (errno == EAGAIN && count++ < 10)
		{
			fprintf(stdout, "count :%d\n", count);
			sleep(1);
			goto tryagain;
		}
		else
		{
			fprintf(stdout, "rules read error\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (n == 0)
	{
		fprintf(stdout, "peer close %s\n", strerror(errno));
	}
	else
	{
		fprintf(stdout, "buf :%s\n", buf);
	}
	write(fd, "1", sizeof("1"));
	return 0;
}
#include <sys/epoll.h>
int epoll_ctl_func(int epfd, int op, int fd, struct epoll_event *event)
{
	int ret;
	if ((ret = epoll_ctl(epfd, op, fd, event)) < 0)
	{
		fprintf(stdout, "epoll_ctl error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int epoll_wait_func(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	int ret;
	if ((ret = epoll_wait(epfd, events, maxevents, timeout)) < 0)
	{
		fprintf(stdout, "epoll wait error %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int accept_func(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int fd;
	if ((fd = accept(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stdout, "accpet error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int main(int argc, char *argv[])
{
	int lifd;
	int fd;
	lifd = create_server();
	//fd = accept(lifd, NULL, NULL);
	//setnonblock(fd);
	//rdwt_once(fd);
	/* create opell */
	int initfd;
	if ((initfd = epoll_create(1)) < 0)
	{
		fprintf(stdout, "create epoll create failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct epoll_event setev;
	setev.data.fd = lifd;
	setev.events = EPOLLIN | EPOLLET;
	epoll_ctl_func(initfd, EPOLL_CTL_ADD, lifd, &setev);
	int rfdn;
#define MAXFD	3
#define TIMEOUT	1000
#define PROMPT	"max link number, letter triy\n"
	struct epoll_event retev[MAXFD];
	int fds[MAXFD - 1];
	int i, cofd;
	int count = 1;
	int n;
	char buf[1024];
	for ( ; ; )
	{
		/* waint file discriptor */
		rfdn = epoll_wait_func(initfd, retev, count, TIMEOUT);
		for (i = 0; i < rfdn; i++)
		{
			/* listen fd operation */
			if (retev[i].data.fd == lifd)
			{
				cofd = accept_func(lifd, NULL, NULL);
				if (count >= MAXFD)
				{
					send(cofd, PROMPT, sizeof(PROMPT), 0);
					close(cofd);
					continue;
				}
				setnonblock(cofd);
				setev.data.fd = cofd;
				setev.events = EPOLLIN | EPOLLET;
				epoll_ctl_func(initfd, EPOLL_CTL_ADD, cofd, &setev);
				count++;
				continue;
			}
			/* fd of socket of client */
			bzero(buf, sizeof(buf));
tryread:
			if ((n = recv(retev[i].data.fd, buf, sizeof(buf), 0)) < 0 && errno == EAGAIN)
			{
				sleep(1);
				goto tryread;
			}
			else if (n == 0)
			{
				fprintf(stdout, "client close \n");
				epoll_ctl_func(initfd, EPOLL_CTL_DEL, retev[i].data.fd, NULL);
				close(retev[i].data.fd);
				count--;
			}
			else if (n > 0)
			{
				fprintf(stdout, "buf :%s\n", buf);
			}
			else
			{
				fprintf(stdout, "read error :%s\n", strerror(errno));
			}
		}
	}
	/* close */
	close(lifd);
	/* main return */
	return 0;
}
