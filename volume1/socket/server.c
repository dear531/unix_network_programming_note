#include <stdio.h>

#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE		50
#define MAXLISTEN	50
#define SERVERPROT	8000

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
	int n;
	int ret;
	printf("start\n");
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenfd < 0)
	{
		printf("socket error\n");
	}


	memset(&servaddr, 0x00, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(SERVERPROT);
	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0)
		printf("bind error\n");

	ret = listen(listenfd, MAXLISTEN);
	if (ret < 0)
		printf("listen error\n");
	struct sockaddr_in scaddr;
	int len;
	connfd = accept(listenfd, (struct sockaddr *)&scaddr, &len);
	if (connfd < 0)
	{
		printf("accept error\n");
		printf("%s", strerror(errno));
	}
	memset(buff, 0x00, MAXLINE);
	while ((n = recv(connfd, buff, MAXLINE, 0)) > 0)
	{
		write(STDOUT_FILENO, buff, n);
		memset(buff, 0x00, MAXLINE);
	}
	putchar('\n');
	close(connfd);

	return 0;	
}
