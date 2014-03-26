#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int fd[FD_SETSIZE];
	int maxfd;
	int i;
	int maxfd;
	listfd = socket();
	maxfd = listfd;
	bind();
	listen();
	for (i = 0; i < FD_SETSIZE; i++)
		fd[i] = -1;
	fd_set rset;
	FD_ZERO(&rset);
	maxi = 0;
	for ( ; ; )
	{
		for (i = 0; i <= maxi; i++)
		{
			/* set read fd set */
			if (fd[i] == -1)
				continue;
			FD_SET(fd[i], &rset);
		}
		FD_SET(maxfd, &rset);
		FD_ISSET(maxfd + 1, &rset);
		ret = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(listfd, &rset))
		{
			connfd = accept();
			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (fd[i] != -1)
					continue;
				fd[i] = connfd;
				if (maxi < i)
					maxi = i;
				break;
			}
			if (i == FD_SETSIZE)
			{
				fprintf(stdout, "too many clients\n");
				exit(EXIT_FAILURE);
			}
			if (connfd > maxfd)
				maxfd = connfd;
		}
	}
	for (i = 0; i <= maxi; i++)
	{
		/*check and headle fd array */
		
	}


	return 0;
}
