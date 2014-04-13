#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc, char *argv)
{
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct timeval opt = 
	{
		opt.tv_sec = 1,
		opt.tv_usec = 0
	};
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(opt)) < 0)
		fprintf(stdout, "setsockopt error :%s\n", strerror(errno));
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	char buf[1024];
	int salen = sizeof(saddr);
	if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
		fprintf(stdout, "sendto error :%s\n", strerror(errno));
	int i;
	bzero(buf, sizeof(buf));
	for (i = 0; i < 10; i++)
		buf[i] = '0' + i;
	fprintf(stdout, "sendto buf :%s\n", buf);
	bzero(buf, sizeof(buf));
	int count = 0;
tryagain:
	if (recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&saddr, &salen) < 0)
		if (errno == EAGAIN)
		{
			if (++count < 10)
			{
				fprintf(stdout, "count :%d\n", count);
				goto tryagain;
			}
			else
			{
				fprintf(stdout, "waiting for data too long time\n");
				exit(EXIT_FAILURE);
			}
		}
	fprintf(stdout, "received buf: %s\n", buf);

	return 0;
}
