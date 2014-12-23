#include <stdio.h>

#include "share.h"

int main(int argc, char *argv[])
{
	/* loop genarate procces */
	int	i;
#define FD_MAX	10
	int	fd[FD_MAX];
	pid_t	pid;
	int	n;
	char	buf[1024];
	signal_func(SIGCHLD, SIG_IGN);
	char c;
	for (i = 0; i < FD_MAX; i++)
	{
		/* fork proccess */
		if ((pid = fork()) < 0)
		{
			fprintf(stdout, "fork error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			/* socket file discriptor */
			fd[i] = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			/* connect */
			struct sockaddr_in	saddr;
			saddr.sin_family 	= AF_INET;
			saddr.sin_port		= htons(8000);
			inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
			connect_func(fd[i], (struct sockaddr *)&saddr, sizeof(saddr));
			for ( ; ; )
			{
				c	= '0' + i;
				n	= send_func(fd[i], &c, sizeof(c), 0);
				recv_func(fd[i], buf, sizeof(buf), 0);
			}
			close(fd[i]);
			fd[i] = -1;
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(fd[i]);
			fd[i] = -1;
		}
	}
	/* close */
	for (i = 0; i < FD_MAX; i++)
		if (fd[i] != -1)
			close(fd[i]);
	return 0;
}

