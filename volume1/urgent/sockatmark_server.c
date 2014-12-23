#include <stdio.h>
#include "share.h"
int create_lisfd(void)
{
	int lisfd = -1;
	/* socket */
	lisfd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* bind */
	struct sockaddr_in saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind_func(lisfd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* listen */
	listen(lisfd, 10);
	return lisfd;
}
int sockatmark_func(int sockfd)
{
	int ret;
	if ((ret = sockatmark(sockfd)) < 0)
	{
		fprintf(stdout, "soctatmark error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}

int main(int argc, char *argv[])
{
	int lisfd = -1;
	/* include socket and bind and listen */
	lisfd = create_lisfd();
	int opt = 1;
	setsockopt_func(lisfd, SOL_SOCKET, SO_OOBINLINE, &opt, sizeof(opt));
	int confd = -1;
	/* accept */
	confd = accept_func(lisfd, NULL, NULL);
	char buf[1024];
	while (1)
	{
		if (sockatmark_func(confd))
			fprintf(stdout, "this first byte is urgent\n");
		bzero(buf, sizeof(buf));
		if (recv_func(confd, buf, sizeof(buf), 0) == 0)
		{
			fprintf(stdout, "peer already close\n");
			close(confd);
			confd = -1;
			break;
		}
		fprintf(stdout, "buf :%s\n", buf);
	}
	/* close connect file descriptor */
	if (confd != -1)
		close(confd);
	/* close listen file descrptor */
	close(lisfd);
	return 0;
}
