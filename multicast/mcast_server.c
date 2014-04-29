#include <unistd.h>

#include "share.h"

int main(int argc, char *argv[])
{
	int fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in maddr;
	maddr.sin_family = AF_INET;
	maddr.sin_port = htons(MULTI_CAST_PORT);
	inet_pton_func(AF_INET, MULIT_CAST_IP, &maddr.sin_addr);
	bind_func(fd, (struct sockaddr *)&maddr, sizeof(maddr));
	char buf[1024];
	for ( ; ; )
	{
		bzero(buf, sizeof(buf));
		sendto(fd, "1", sizeof("1"), 0, (struct sockaddr *)&maddr, sizeof(maddr));
		recvfrom_func(fd, buf, sizeof(buf), 0, NULL, NULL);
		fprintf(stdout, "buf: %s\n", buf);
	}

	return 0;
}
