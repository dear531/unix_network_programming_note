#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	/* socketpair */
	int inv[2];
	if ((socketpair(AF_LOCAL, SOCK_STREAM, 0, inv)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* fork */
	pid_t pid;
	pid = fork();
	int status;
	char strfd[5];
	if (pid < 0)
	{ /* fork error */
		fprintf(stdout, "fork error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{ /* child */
		close(inv[0]);
		bzero(strfd, sizeof(strfd));
		snprintf(strfd, sizeof(strfd), "%d", inv[1]);
		execl("getfd", "./getfd", strfd, NULL);
		close(inv[1]);
	}
	else
	{ /* prenet */
		close(inv[1]);
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) == 0)
		{
			fprintf(stdout, "child normal tarminal\n");
		}
		else
		{
			errno = WEXITSTATUS(status);
			fprintf(stdout, "child exit status :%s\n", strerror(errno));
		}
		close(inv[0]);
	}
	return 0;
}
