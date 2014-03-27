#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
/* create listen file descriptor */
int listen_conn(void)
{
	int listfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listfd < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	int maxfd;
	maxfd = listfd;

	int ret;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	if ((ret = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr)) == -1)
	{
		fprintf(stdout, "ip pinter control network byte failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		fprintf(stdout, "bad ipv4 format\n");
		exit(EXIT_FAILURE);
	}
	if (bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (listen(listfd, FD_SETSIZE) < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return listfd;
}



int main(int argc, char *argv[])
{
	int listfd;
	int maxfd;
	listfd = listen_conn();
	maxfd = listfd;
	int fd[FD_SETSIZE];
	fd_set rset;
	fd_set wset;
	int connfd;
	char buff[1024];
	char prompt[256];
	int n;
	int i;
	for (i = 0; i < FD_SETSIZE; i++)
		fd[i] = -1;
	int maxi;
	maxi = 0;
	int ret;

	for ( ; ; )
	{
		FD_ZERO(&rset);

		/* add listen descriptor into read set */
		FD_SET(listfd, &rset);

		/* add client descriptor into read set */
		for (i = 0; i <= maxi; i++)
		{
			/* set read fd set */
			if (fd[i] == -1)
				continue;
			FD_SET(fd[i], &rset);
		}

		ret = select(maxfd + 1, &rset, NULL, NULL, NULL);
		/* accept new client socket */
		if (FD_ISSET(listfd, &rset))
		{
			if ((connfd = accept(listfd, NULL, NULL)) < 0)
			{
				fprintf(stdout, "accpet error :%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (fd[i] != -1)
					continue;

				/* save new link */
				fd[i] = connfd;

				/* update loop maximum for file descriptor of client */
				if (maxi < i)
					maxi = i;

				/* update maximum file descriptor for select function */
				if (maxfd < connfd)
					maxfd = connfd;
				break;
			}

			/* check maximum client counter */
			if (i == FD_SETSIZE)
			{
				fprintf(stdout, "too many clients\n");
				exit(EXIT_FAILURE);
			}
		}

		/* check to readable for descriptor */
		for (i = 0; i <= maxi; i++)
		{
			/* check counter fo readable decsript file */
			if (ret <= 0)
				break;

			/*check and headle fd array */
			if (!FD_ISSET(fd[i], &rset))
				continue;

			/* read handle */
			bzero(buff, sizeof(buff));
			n = recv(fd[i], buff, sizeof(buff), 0);
			if (n < 0)
			{
				fprintf(stdout, "recv error :%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			else if(n == 0)
			{
				close(fd[i]);
				fd[i] = -1;
				/* update maximum loop counter and file descriptor counter */
				if (i == maxi)
				{
					maxi--;
					maxfd--;
				}
			}
			bzero(prompt, sizeof(prompt));
			sprintf(prompt, "received %d byte\n", n);
			fprintf(stdout, "%s\n", prompt);

			/* handle maximum of readable descriptor */
			ret--;

			/* write handle */
			if (fd[i] == -1)
				continue;
			FD_ZERO(&wset);
			FD_SET(fd[i], &wset);
			select(fd[i] + 1, NULL, &wset, NULL, 0);
			if (FD_ISSET(fd[i], &wset))
				send(fd[i], prompt, strlen(prompt), 0);
		}

		/* show array of file descriptor */
		fprintf(stdout, "maxi:%d, maxfd:%d\n", maxi, maxfd);
		for (i = 0; i < FD_SETSIZE; i++)
		{
			if (i % 50 == 0)
				fprintf(stdout, "%2d:", i / 50 + 1);
			fprintf(stdout, "%2d", fd[i]);
			if (i % 50 == 49)
				fprintf(stdout, "\n");
		}
	}
	/* close all client link */
	for (i = 0; i < maxi; i++)
	{
		if (fd[i] == -1)
			continue;
		close(fd[i]);
		fd[i] = -1;
	}
	close(listfd);
	listfd = -1;

	return 0;
}
