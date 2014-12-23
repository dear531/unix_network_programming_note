#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char *argv[])
{
	/* ignore signal SIGCHLD */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		fprintf(stdout, "set ignore chld faile :%s\n", strerror(errno));
	/* create socket pair */
	int fds[2];
	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, fds) < 0)
	{
		fprintf(stdout, "socketpair :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* fork create process pair for perent and child */
	pid_t pid;
	char sbuf[1024], rbuf[1024];
	int i, n;
	if ((pid = fork()) < 0)
	{
		fprintf(stdout, "fork error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		close(fds[0]);
		while (1)
		{
			bzero(rbuf, sizeof(rbuf));
			recv(fds[1], rbuf, strlen(rbuf), 0);
			bzero(sbuf, sizeof(sbuf));
			sbuf[0] = '6';
			send(fds[1], sbuf, strlen(sbuf), 0);
			sleep(1);
		}
		close(fds[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fds[1]);
		for (i = 0; i < 5; i++)
		{
			sbuf[0] = '0' + i;
			bzero(sbuf, sizeof(sbuf));
			send(fds[0], sbuf, sizeof(sbuf), 0);
			bzero(rbuf, sizeof(rbuf));
			if ((n = recv(fds[0], rbuf, sizeof(rbuf), 0)) == 0)
				break;
			else if (n < 0)
				exit(EXIT_FAILURE);
			else
				fprintf(stdout, "received rbuf :%s\n", rbuf);
		}
		close(fds[0]);
		exit(EXIT_SUCCESS);
	}
	/* received and send data */
	/* close pair socket */
	return 0;
}
