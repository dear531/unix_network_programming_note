#include <stdio.h>

int main(int argc, char *argv[])
{
	/* signal ignore SIGCHLD */
	int lifd, cofd;
	/* socket */
	cofd = socket();
	/* bind */
	bind();
	/* listen */
	listen();
	for ( ; ; )
	{
		/* accept */
		cofd = accept();
		/* fork child */
		if ((pid = fork()) < 0)
		{
			fprintf(stdout, "fo");
		}
		else if (pid > 0)
		{
		/* perent operation */
			close(cofd);
		}
		else
		{
			close(lifd);
		/* child operation */
		}
	}
	return 0;
}
