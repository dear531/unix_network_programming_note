#include <stdio.h>

#include "share.h"

int create_listen()
{
	int fd;
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int opt = 1;	
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));

	listen_func(fd, 10);

	return fd;
}

void *pfunc(void *arg)
{
	pthread_detach(pthread_self());
	char buf[1024];
	int n;
	for ( ; ; )
	{
		bzero(buf, sizeof(buf));
		if ((n = recv((int)arg, buf, sizeof(buf), 0)) == 0)
		{
			close((int)arg);
			return (NULL);
		}
		send((int)arg, buf, n, 0);
	}
	close((int)arg);
	return (NULL);
}

int main(int argc, char *argv[])
{
	int lifd, cofd;
	/* combine :socket and bind adn listen */
	lifd = create_listen();
	for ( ; ; )
	{
		/* accept */
		cofd = accept_func(lifd, NULL, NULL);
		write(STDOUT_FILENO, ".", sizeof("."));
		pthread_t pt;
		/* create new pthead */
		pthread_create(&pt, NULL, pfunc, (void *)cofd);
	}
	return 0;
}
