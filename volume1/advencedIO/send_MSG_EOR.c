#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>


int main(int argc, char *argv)
{
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	if (connect(fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
		fprintf(stdout, "connect error :%s\n", strerror(errno));
	sleep(20);
	char buf[1024];
	int i;
	bzero(buf, sizeof(buf));
	for (i = 0; i < 10; i++)
		buf[i] = '0' + i;
#if 0
	send(fd, buf, sizeof(buf), MSG_EOR);
#else
	send(fd, buf, sizeof(buf), 0);
	shutdown(fd, SHUT_RDWR);
#endif
	fprintf(stdout, "send buf :%s\n", buf);
	sleep(30);
	close(fd);
	return 0;
}
