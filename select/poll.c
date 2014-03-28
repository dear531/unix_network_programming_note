#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int listenfd(void)
{
	int listfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	int ret;
	ret = bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	listen(listfd, 10);
	fprintf(stdout, "listfd show value :%d\n", listfd);
	return listfd;
}

int main(int argc, char *argv[])
{
	struct pollfd fds[1024];
	fds[0].fd = listenfd();

	int i;
	int nread;

	for (i = 1; i < 1024; i++)
		fds[i].fd = -1;

	int maxi = 0;
	char buf[1024];
	int connfd;
	int n;
	char tmp[256];
	fds[0].events = POLLIN;
	for ( ; ; )
	{
#if 0
		fprintf(stdout, "maxi :%d\n", maxi);
		for (i = 0; i < 1024; i++)
		{
			if (i % 50 == 0)
				fprintf(stdout, "%2d:", i / 50 + 1);
			fprintf(stdout, "%2d", fds[i].fd);
			if (i % 50 == 49)
				putchar('\n');
		}
		putchar('\n');
#endif
		nread = poll(fds, maxi + 1, -1);
		if (nread < 0)
		{
			fprintf(stdout, "poll error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if (fds[0].revents & POLLIN)
		{
			connfd = accept(fds[0].fd, NULL, NULL);
			for (i = 1; i < 1024; i++)
			{
				if (fds[i].fd == -1)
				{
					fds[i].fd = connfd;
					fds[i].events = POLLIN;
					if (maxi < i)
						maxi = i;
					break;
				}
			}
			if (--nread <= 0)
				continue;
		}
		/* client handle */
		for (i = 1; i <= maxi; i++)
		{
			fprintf(stdout, "(fds[i].revents & POLLIN) == 0 :%d\n", (fds[i].revents & POLLIN) == 0);
			fprintf(stdout, "fds[i].revents & POLLIN == 0 :%d\n", fds[i].revents & POLLIN == 0);
			if ((fds[i].revents & POLLIN) == 0)
				continue;
			/* read client descript handle */
			bzero(buf, sizeof(buf));
			n = recv(fds[i].fd, buf, sizeof(buf), 0);
			if (n < 0)
			{
				fprintf(stdout, "recv error :%s\n", strerror(errno));
			}
			else if (n == 0)
			{
				fprintf(stdout, "here unlink\n");
				close(fds[i].fd);
				fds[i].fd = -1;
				fds[i].revents = 0;
				while (fds[maxi].fd == -1)
					maxi--;
				continue;
			}
			else
			{
				fprintf(stdout, "fd %d recv size %ld buf :%s\n", i, strlen(buf), buf);
			}
			/* write client descript handle */
			bzero(tmp, sizeof(tmp));
			sprintf(tmp, "server received size %ld byte\n", strlen(buf));
			send(fds[i].fd, tmp, strlen(tmp), 0);
			if (--nread <= 0)
				break;
		}
	}
	for (i = 1; i <= maxi; i++)
	{
		close(fds[i].fd);
		fds[i].fd = -1;
	}
	close(fds[0].fd);
	fds[0].fd = -1;
	return 0;
}
