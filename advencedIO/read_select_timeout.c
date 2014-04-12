#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	if (connect(fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "create file descriptor :%d\n", fd);
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	fd_set set;
	FD_ZERO(&set);
	FD_SET(fd, &set);
	select(fd + 1, &set, NULL, NULL, &tv);
	fprintf(stdout, "10 second read returned\n");
	
	return 0;
}
