#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int creat_listen()
{
	int listfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	int ret;
	ret = bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	listen(listfd, 10);
	return listfd;
}
#include <unistd.h>
#include <fcntl.h>

void setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

#include <sys/epoll.h>

#define FDMAX	3
int main(int argc, char *argv[])
{
	int listfd;
	listfd = creat_listen();
	/* 1 create epoll fd */
	int epfd;
	epfd = epoll_create(50);
	/* 2 add listen in set of event */
#if 0
	struct epoll_event {
		uint32_t     events;      /* Epoll events */
		epoll_data_t data;        /* User data variable */
	};

#endif
	/* 3 loop start -- handle connect and client */
	struct epoll_event setev, retev[FDMAX];
	setev.data.fd = listfd;
	setev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listfd, &setev);
	int ret, i;
	int connfd;
	char rbuf[1024], sbuf[1024];
	int count = 0;
	int clientnum = 0;
	int n;
	int m = 0;
	char *buys = "service busy, please littel try\n";
	for ( ; ; )
	{
		fprintf(stdout, "count :%d\n", count++);
		/*
		 * received one server, alread when client send multi.
		 * check and show server combined received.
		 */
#define CLIET_SEPARATELY_TO_SERVER_COMBINED 0
#if CLIET_SEPARATELY_TO_SERVER_COMBINED
		fprintf(stdout, "sleep 5 second wait data\n");
		sleep(5);
#endif
		ret = epoll_wait(epfd, retev, FDMAX, -1);
		for (i = 0; i < ret; i++)
		{
			if (retev[i].data.fd == listfd)
			{
				/* check and connect new client, and new cliet fd add event set */
				connfd = accept(retev[i].data.fd, NULL, NULL);
				if (clientnum >= FDMAX - 1)
				{
					fprintf(stdout, "client too many\n");
					send(connfd, buys, strlen(buys), 0);
					close(connfd);
					continue;
				}
				setnonblocking(connfd);
				setev.data.fd = connfd;
				setev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &setev);

				fprintf(stdout, "accept client fd :%d\n", connfd);
				clientnum++;
				continue;

			}
#if 0
			if(retev[i].events & (EPOLLIN))
			{
				/* check and read client */
				bzero(rbuf, sizeof(rbuf));
				/*
				 * not readed received buffer, try check readable,
				 * should unreadable, under OPOLLET module.
				 */
#define CHECK_EPOLLIN_EPOLLET 1
#if CHECK_EPOLLIN_EPOLLET 
				recv(retev[i].data.fd, rbuf, 1, 0);
#else
				n = recv(retev[i].data.fd, rbuf, 1024, 0);
				if (n == 0)
				{
					close(retev[i].data.fd);
					//	retev[i].data.fd = -1;
					fprintf(stdout, "client close\n");
					clientnum--;
				}
#endif
				fprintf(stdout, "received to fd %d rbuf :%s\n", retev[i].data.fd, rbuf);
			}
#endif
#if 1
			if (retev[i].events & EPOLLOUT)
			{
				/* check and write client */
				bzero(sbuf, sizeof(sbuf));
				sprintf(sbuf, "is fd %d send\n", retev[i].data.fd);
				fprintf(stdout, "send to fd :%d\n", retev[i].data.fd);
				/*
				 * check file descrptor is level trigeredd set is ro not keep.
				 * if level trigered whenever appear empty, when check writeable is true.
				 * if edge trigered only first appear empy, when check first is true, other time is flase.
				 */
#define CHECK_EPOLLOUT_LT 1
#if CHECK_EPOLLOUT_LT
				m = 0;
				while (1)
				{
					n = send(retev[i].data.fd, sbuf, strlen(sbuf), 0);
					if (n < 0 && errno == EAGAIN)
						break;
					m += n;
				}
				fprintf(stdout, "m:%d\n", m);
#endif
			}
#endif
		}
	}
	/* close epoll fd */

	return 0;
}
