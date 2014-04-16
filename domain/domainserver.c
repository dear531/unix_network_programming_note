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


int main(int argc, char *argv[])
{
	/* signal ignore SIGCHLD */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		fprintf(stdout, "set ignore failed\n");
	/* socket */
	int lifd, cofd;
	if ((lifd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind */
#define UNIX_PATH	"unix_tmp"
	char * fpath = UNIX_PATH;
	int ret;
	struct sockaddr_un un_addr;
	un_addr.sun_family = AF_LOCAL;
	bzero(un_addr.sun_path, sizeof(un_addr.sun_path));
	bcopy(fpath, un_addr.sun_path, strlen(fpath) > sizeof(un_addr.sun_path) - 1	\
			? sizeof(un_addr.sun_path) - 1 : strlen(fpath));
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
