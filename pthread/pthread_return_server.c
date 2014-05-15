#include <stdio.h>
#include <pthread.h>

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
	int ret;
	if ((ret = pthread_detach(pthread_self())) > 0)
		fprintf(stdout, "pthread error :%s\n", strerror(ret));
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
	int ret;
	for ( ; ; )
	{
		/* accept */
		cofd = accept_func(lifd, NULL, NULL);
		write(STDOUT_FILENO, ".", sizeof("."));
		pthread_t pt;
		/* create new pthead */
#if 0		/* test return value of pthread function */
		char test;
		if ((ret = pthread_create(&pt, (const pthread_attr_t *)&test, pfunc, (void *)cofd)) > 0)
#endif
		if ((ret = pthread_create(&pt, NULL, pfunc, (void *)cofd)) > 0)
			fprintf(stdout, "pthread_create error :%s\n", strerror(ret));
	}
	return 0;
}
