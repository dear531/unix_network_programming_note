#include <stdio.h>
#include <pthread.h>

#include "share.h"

ssize_t read_func(int fd, void *buf, size_t count)
{
	int n;
	if ((n = read(fd, buf, count)) < 0)
	{
		fprintf(stdout, "read error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return n;
}

int fd;
void *pfunc (void *arg)
{
	char	buf[1024];
	int	n;
	while ((bzero(buf, sizeof(buf)), n = read_func(STDIN_FILENO, buf, sizeof(buf))) != 0)
	{
		send(fd, buf, n, 0);
	}
	shutdown(fd, SHUT_WR);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));

	pthread_t pt = 0;
	pthread_create(&pt, NULL, pfunc, NULL);

	char buf[1024];
	int n;
	while ((bzero(buf, sizeof(buf)), n = recv(fd, buf, sizeof(buf), 0)) != 0)
	{
		write(STDOUT_FILENO, buf, n);
	}
	exit(EXIT_SUCCESS);

	return 0;
}
