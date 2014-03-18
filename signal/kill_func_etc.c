#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#if 0
int main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "sleep(%d)\n", i + 1);
		sleep(1);
	}
	kill(getpid(), SIGKILL);
	raise(SIGKILL);
	return 0;
}
#else
int main(int argc, char *argv[])
{
	pid_t pid;
	int i, j;
	for (i = 0; i < 5; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			/* child process */
			if (i == 4)
			{
				sleep(5);
				//kill(0, SIGKILL);
				fprintf(stdout, "is %d\n", getpid());
			}
			for (j = 0; j < 10; j++)
			{
				fprintf(stdout, "I'm child %d sleep(%d)\n", getpid(), j + 1);
				sleep(1);
			}
			exit(0);
		}
		else if (pid > 0)
		{
			/* parent process */
			fprintf(stdout, "fork process %d\n", i + 1);
		}
		else
		{
			/* fork error */
			fprintf(stdout, "fork error:%s\n", strerror(errno));
		}
	}
	sleep(5);
	kill(0, SIGKILL);
	sleep(30);
	return 0;
}
#endif
