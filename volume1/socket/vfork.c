#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include "bit_check.h"

int main(void)
{
	pid_t pid;
	int i;
	pid = fork();
	int status;
#if 1
	if (pid == 0)
	{
		/* is child */
		//for (i = 0; i < 10; i++)
		{
			fprintf(stdout, "sleep:%d\n", i);
			//	sleep(1);
		}
		fprintf(stdout, "child process: child pid:%d\n", getpid());
		exit(EXIT_SUCCESS);
	}
	else if (pid > 0)
	{
		/* is parent */
		fprintf(stdout, "parent process: child pid:%d\n", pid);
		//pid = wait(&status);
		bit_check(&status, sizeof(status));
	}
	else
	{
		/* is error */
		fprintf(stdout, "error:%s\n", strerror(errno));
	}
#endif
	return 0;
}
