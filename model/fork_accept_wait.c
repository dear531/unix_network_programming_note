#include <stdio.h>

#include "share.h"

int create_listen()
{
	/* socket */
	int	fd;
	fd	= socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set address reuse */
	int	opt	= 1;
	setsockopt_func(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	/* bind */
	struct sockaddr_in	saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* listlen */
	listen_func(fd, 10);

	return fd;
}
#include <sys/time.h>
#include <sys/resource.h>
void pr_cpu_time(void)
{
	struct rusage	parent, child;
	if ((getrusage(RUSAGE_SELF, &parent)) < 0)
	{
		fprintf(stdout, "get parent resource prefrom failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if ((getrusage(RUSAGE_CHILDREN, &child)) < 0)
	{
		fprintf(stdout, "get child resource prefrom failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
	struct timeval {
		long tv_sec;        /* seconds */
		long tv_usec;       /* microseconds */
	};

#endif
	long	utime, stime;
	utime	= (parent.ru_utime.tv_sec + child.ru_utime.tv_sec) * (1000 * 1000)
		+ (parent.ru_utime.tv_usec + child.ru_utime.tv_usec);
	fprintf(stdout, "user time :%ld\n", utime);
	stime	= (parent.ru_stime.tv_sec + child.ru_stime.tv_sec) * (1000 * 1000)
		+ (parent.ru_utime.tv_usec + child.ru_utime.tv_usec);
	fprintf(stdout, "sys time :%ld\n", stime);
	exit(EXIT_SUCCESS);
		return;
}
#define FD_MAX	1000
pid_t	pid[FD_MAX];
void sig_int(int signum)
{
	int i;
	for (i = 0; i < FD_MAX; i++)
		kill(SIGTERM, pid[i]);
	for (i = 0; i < FD_MAX; i++)
		waitpid( pid[i], NULL, -1);
	pr_cpu_time();
	exit(EXIT_SUCCESS);
}
void sig_pipe(int signum)
{
	return;
}
int main(int argc, char argv[])
{
	int	lifd, cofd;
	lifd	= create_listen();
	int	i;
	char	buf[1024];
	int	n;
	signal_func(SIGCHLD, SIG_IGN);
	signal_func(SIGPIPE, sig_pipe);
	fd_set rset;
	for (i = 0; i < FD_MAX; i++)
	{
		if ((pid[i] = fork()) < 0)
		{
			fprintf(stdout, "fork error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else if (pid[i] == 0)
		{
			FD_ZERO(&rset);
			FD_SET(lifd, &rset);
			fprintf(stdout, "i :%d\n", i);
			select_func(lifd + 1, &rset, NULL, NULL, NULL);
			fprintf(stdout, "return i :%d\n", i);
			cofd	= accept(lifd, NULL, NULL);
			for ( ; ; )
			{
				bzero(buf, sizeof(buf));
				if ((n = recv_func(cofd, buf, sizeof(buf), 0)) == 0)
				{
					fprintf(stdout, "peer close\n");
					exit(EXIT_SUCCESS);
				}
				write(STDOUT_FILENO, buf, sizeof(buf));
				send_func(cofd, buf, n, 0);
			}
			exit(EXIT_SUCCESS);
		}
	}
	signal_func(SIGINT, sig_int);
	for ( ; ; )
		pause();

	return 0;
}
