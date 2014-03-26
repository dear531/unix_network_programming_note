#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

int max(int a, int b)
{
	return a > b ? a : b;
}

int main(int argc, char *argv[])
{
	fd_set rset;
	fd_set wset;
	fd_set eset;
	int maxfd;
	int ret;

	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(STDIN_FILENO, &rset);
	FD_SET(STDOUT_FILENO, &wset);
	maxfd = max(STDIN_FILENO, STDOUT_FILENO);
	int flags;
	char buf[1024];
	for ( ; ; )
	{
		flags = 0;
		//fprintf(stdout, "here befoer select\n");
		ret = select(maxfd + 1, &rset, &wset, NULL, NULL);
		//fprintf(stdout, "here after select\n");
		fprintf(stdout, "ret :%d\n", ret);
		fprintf(stdout, "stdout:%d, stdin:%d, maxfd:%d\n", STDOUT_FILENO, STDIN_FILENO, maxfd);
		if (FD_ISSET(STDIN_FILENO, &rset))
		{
			read(STDIN_FILENO, buf, sizeof(buf));
			fprintf(stdout, "read buf:%s\n", buf);
			flags = 1;
		}

		if (FD_ISSET(STDOUT_FILENO, &wset) && flags)
		{
			write(STDOUT_FILENO, "is write \n", sizeof("is write \n"));
		}
		sleep(1);
	}
	return 0;
}
