#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int main(int argc, char *argv[])
{
	int listfd;
	int maxfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	fd_set set;

	maxfd = listfd;
	FD_ZERO(&set);
	FD_SET(listfd, &set);
	select(maxfd + 1, &set, NULL, NULL, NULL);
	/* socket create immediately return */
	fprintf(stdout, "here socket fd readable\n");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind(listfd, (struct sockaddr*)&saddr, sizeof(saddr));

	FD_ZERO(&set);
	FD_SET(listfd, &set);
	select(maxfd + 1, &set, NULL, NULL, NULL);
	/* bind create immediately return */
	fprintf(stdout, "here bind fd readable\n");

	listen(listfd, 10);
	FD_ZERO(&set);
	FD_SET(listfd, &set);
	select(maxfd + 1, &set, NULL, NULL, NULL);
	/* renturn when client connect  */
	fprintf(stdout, "here listen fd readable\n");

	int connfd;
	connfd = accept(listfd, NULL, NULL);
	fprintf(stdout, "check avalable fd:%d\n", connfd);
	maxfd = connfd;
	FD_ZERO(&set);
	FD_SET(connfd, &set);
	select(maxfd + 1, &set, NULL, NULL, NULL);
	/* return when client send data */
	fprintf(stdout, "here accept fd readable\n");

	return 0;
}
