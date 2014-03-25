#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if 0
int str_cli(int *fd)
{
	int ret;
	int i;
	char buf[1024];
	for ( ; ; )
	{
		fd_set rset;
		FD_ZERO(&rset);
		int fd[10];
		for (i = 0; i < 10; i++)
		{
			FD_SET(fd[i], &rset);
			fprintf(stdout, "set stdin set\n");
		}
		ret = select(10 + 1, NULL, &rset, NULL, NULL);

		for (i = 0; i < ret; i++)
		{
			fprintf(stdout, "is start read\n");
			if (FD_ISSET(fd[i], &rset))
			{
				while (gets(buf) != NULL)
					fputs(buf, stdout);
			}
			else
			{
				i++;
			}
			fprintf(stdout, "is end read\n");
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

int
main(int argc, char *argv[])
{
	int listfd, connfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sadd;
	sadd.sin_family = AF_INET;
	sadd.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &sadd.sin_addr);
	bind(listfd, (struct sockaddr *)&sadd, sizeof(sadd));

	listen(listfd, 10);
	char buf[1024];
	int n;
	pid_t pid;
	for ( ; ; )
	{
		connfd = accept(listfd, NULL, NULL);
		if (connfd < 0)
			fprintf(stdout, "accept error:%s\n", strerror(errno));
#if 0
		while (1)
		{
			n = recv(connfd, buf, 1024, 0);
			send(connfd, buf, n, 0);
		}
		close(connfd);
		connfd = -1;
#else
		pid = fork();
		if (pid == 0)
		{
			close(listfd);
			listfd = -1;
			while (1)
			{
				n = recv(connfd, buf, 1024, 0);
				send(connfd, buf, n, 0);
			}
			sleep(10);
			/* child handle */
			close(connfd);
			connfd = -1;
			exit(EXIT_SUCCESS);
		}
		else
		{
			/* perant handle */
			/*
			close(connfd);
			connfd = -1;
			*/
		}
#endif
	}
	close(listfd);
	listfd = -1;
	
	return 0;
}
