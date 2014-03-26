#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>


#if 1
int str_cli(int *fd, int n)
{
	int ret;
	int i;
	char buf[1024];
	int m;
	int prev;
	int count;
	for (i = 0; i < n; i++)
		fprintf(stdout, "fd[%d]:%d\n", i, fd[i]);
	bzero(buf, 1024);
	prev = 0;
	fd_set rset;
	fd_set wset;
	int flags;
	char prompt[1024];
	for ( ; ; )
	{
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		for (i = 0; i < n; i++)
		{
			FD_SET(fd[i], &rset);
			FD_SET(fd[i], &wset);
		}

		ret = select(fd[n - 1] + 1, &rset, &wset, NULL, NULL);

		count = 0;
		for (i = 0; i < n; i++)
		{
			flags = 0;
			if (FD_ISSET(fd[i], &rset))
			{
readagain:
				m = recv(fd[i], buf, 1024, 0);
				if (m < 0)
					fprintf(stdout, "recv error:%s\n", strerror(errno));
				else if (m == 0)
					fprintf(stdout, "peer pointer cloese\n");

				/* handle receive buffer remains */
				if (m >= 0 && prev > m)
					bzero(buf + m, prev - m);

				/* recore previous length of buffer */
				if (m >= 0 && prev != m)
					prev = m;
				fprintf(stdout, "fd:%d, buf:%s", fd[i], buf);

				/* check isn't read end */
				if (m == 1024)
					goto readagain;

				flags = 1;

				/* check number of descriptor */
				if (++count == ret)
					break;
			}
			if (FD_ISSET(fd[i], &wset) && flags)
			{
				flags = 0;
				sprintf(prompt, "receive buff %d byte\n", m);
				send(fd[i], prompt, strlen(prompt), 0);

				/* check number of descriptor */
				if (++count == ret)
					break;
			}
		}
	}
	return 0;
}
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
void tmpfunc(int *connfd, int num)
{
	int n;
	char buf[1024];
	int i;
	for (i = 0; i < num; i++)
	{
		n = recv(connfd[i], buf, 1024, 0);
		send(connfd[i], buf, n, 0);
	}
	return;
}
#define MAXFD	2
	int
main(int argc, char *argv[])
{
	int ret;

	int listfd, connfd[MAXFD];
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sadd;
	sadd.sin_family = AF_INET;
	sadd.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &sadd.sin_addr);
	ret = bind(listfd, (struct sockaddr *)&sadd, sizeof(sadd));
	if (ret < 0)
	{
		fprintf(stdout, "bind error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	listen(listfd, MAXFD);
	char buf[1024];
	int n;
	pid_t pid;
	int i;
	for (i = 0; i < MAXFD; i++)
	{
		connfd[i] = accept(listfd, NULL, NULL);
		if (connfd[i] < 0)
		{
			fprintf(stdout, "accept error:%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	//	tmpfunc(connfd, MAXFD);
	str_cli(connfd, MAXFD);
	for (i = 0; i < MAXFD; i++)
	{
		close(connfd[i]);
		connfd[i] = -1;
	}
	close(listfd);
	listfd = -1;

	return 0;
}
