#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in saddr;
	int ret;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	/* must bind any address, othewise nothing received */
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char buf[1024];
	for ( ; ; )
	{
		bzero(buf, sizeof(buf));
		recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		write(STDOUT_FILENO, buf, strlen(buf));
	}
	
	return 0;
}
