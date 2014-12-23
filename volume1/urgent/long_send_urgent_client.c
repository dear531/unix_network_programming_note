#include <stdio.h>

#include "share.h"

int main(int argc, char *argv[])
{
	/* socket */
	int confd;
	confd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set send buf size */
	int size = 1024*10*2;
	setsockopt_func(confd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
	/* connect */
	struct sockaddr_in	saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect_func(confd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* loop send and recv */
	char buf[1024 * 10];
	sleep(1);
	memset(buf, 0xff, sizeof(buf));
	send_func(confd, buf, sizeof(buf), 0);
	fprintf(stdout, "send normal send done\n");
	send_func(confd, "u", sizeof("u"), MSG_OOB);
	sleep(30);
	/* close */
	close(confd);
	return 0;
}
