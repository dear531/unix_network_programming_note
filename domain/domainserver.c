#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#define UNIX_PATH	"/tmp/unix_tmp"
void sin_int(int signum)
{
	int sa_er;
	sa_er = errno;
	unlink(UNIX_PATH);
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		fprintf(stdout, "set SIGINT SIG_DFL failed\n");
	errno = sa_er;
	return;
}
static inline int
min(int m, int n)
{
	return (m < n ? m : n);
}
int main(int argc, char *argv[])
{
	/* signal ignore SIGCHLD */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		fprintf(stdout, "set ignore failed\n");
	if (signal(SIGINT, sin_int) == SIG_ERR)
		fprintf(stdout, "set SIGINT failed\n");
	/* socket */
	int lifd, cofd;
	if ((lifd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind */
	char * fpath = UNIX_PATH;
	int ret;
	struct sockaddr_un un_addr;
	bzero(&un_addr, sizeof(un_addr));
	un_addr.sun_family = AF_LOCAL;
	bcopy(fpath, un_addr.sun_path, min(strlen(fpath), sizeof(un_addr.sun_path) - 1));
	unlink(fpath);
	if ((ret = bind(lifd, (struct sockaddr*)&un_addr, sizeof(un_addr))) < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
	if ((ret = listen(lifd, 10)) < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_un un_caddr;
	socklen_t clen = sizeof(un_caddr);
	pid_t pid;
	char rbuf[1024], sbuf[1024];
	int count;
	for ( ; ; )
	{
		count = 0;
		/* accept */
		if ((cofd = accept(lifd, (struct sockaddr *)&un_caddr, &clen)) < 0)
		{
			fprintf(stdout, "accept error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		/* fork child */
		if ((pid = fork()) < 0)
		{
			fprintf(stdout, "fork errror :%s\n", strerror(errno));
		}
		else if (pid > 0)
		{
			/* perent operation */
			unlink(UNIX_PATH);
			close(cofd);
			cofd = -1;
		}
		else
		{
			/* child operation */
			close(lifd);
			lifd = -1;
			while (bzero(rbuf, sizeof(rbuf)), (ret = recv(cofd, rbuf, sizeof(rbuf), 0)) > 0) 
			{
				fprintf(stdout, "received buf :%s", rbuf);
				bzero(sbuf, sizeof(sbuf));
				sprintf(sbuf, "received counter :%d\n", count++);
				send(cofd, sbuf, strlen(sbuf), 0);
				sleep(20);
			}
			if (ret == 0)
				fprintf(stdout, "client close\n");
			else
				fprintf(stdout, "received error :%s\n", strerror(errno));
			close(cofd);
			cofd = -1;
			unlink(fpath);
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}
