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

int main(void)
{
	int lisfd, confd;
	lisfd = create_lisfd();
#if 1
	confd = accept_func(lisfd, NULL, NULL);
#else
	confd = accept4_func(lisfd, NULL, NULL, SOCK_NONBLOCK);
#endif
	fd_set rset, xset;
	char buf[1024];
	int justreadoob;
	FD_ZERO(&rset);
	FD_ZERO(&xset);
	int n;
	for ( ; ; )
	{
		FD_SET(confd, &rset);
		FD_SET(confd, &xset);
		select_func(confd + 1, &rset, NULL, &xset, NULL);
		if (FD_ISSET(confd, &rset))
		{
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
		if(FD_ISSET(confd, &xset))
		{
			bzero(buf, sizeof(buf));
			recv_func(confd, buf, sizeof(buf), MSG_OOB);
			fprintf(stdout, "urgent data buf :%s\n", buf);
		}
	}
	if (confd != -1)
		close(confd);
	close(lisfd);
	return 0;
}
