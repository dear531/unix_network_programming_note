#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char argv[])
{
	int listfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listfd < 0)
	{
		fprintf(stdout, "socket error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	int ret;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port	= htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	ret = bind(listfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = listen(listfd, 10);
	if (ret < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	int connfd;
	connfd = accept(listfd, NULL, NULL);
	if (connfd < 0)
	{
		fprintf(stdout, "accept error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "check avalable connfd:%d\n", connfd);
	int n;
	char buf[1024];
	int nrd;
	nrd = 0;
	bzero(buf, sizeof(buf));
	while (1)
	{
		n = recv(connfd, buf, sizeof(buf), 0);
		if (n < 0)
		{
			fprintf(stdout, "receive error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else if(n == 0)
		{
			fprintf(stdout, "peer close\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "received buf :%s\n", buf);
		n = send(connfd, buf, n, 0);
		if (n < 0)
		{
			fprintf(stdout, "send error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else if (n == 0)
		{
			fprintf(stdout, "peer close\n");
			exit(EXIT_FAILURE);
		}

		shutdown(connfd, SHUT_WR);
	}
	
	return 0;
}
