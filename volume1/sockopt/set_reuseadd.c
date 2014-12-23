#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stdout, "paramenter numaric error\n");
		exit(EXIT_FAILURE);
	}
	int sockfd, connfd;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int opt = 1;
	int optlen = sizeof(opt);
#if 0
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
#endif
	getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, &optlen);
	fprintf(stdout, "default use address opt value:%d\n", opt);
	struct sockaddr_in saddr;

	int ret;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &saddr.sin_addr);
	ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind falied :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	listen(sockfd, 10);
	connfd = accept(sockfd, NULL, NULL);
	sleep(100);
	close(connfd);
	close(sockfd);
	return 0;
}
