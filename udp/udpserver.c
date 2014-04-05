#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;
void sig_int(int signum)
{
	fprintf(stdout, "count :%d\n", count);
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
	signal(SIGINT, sig_int);
	int udpfd;
	udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind(udpfd, (struct sockaddr*)&saddr, sizeof(saddr));
	int caddrlen;
	char rbuf[1024], sbuf[1024];
	while(1)
	{
		count++;
		caddrlen = sizeof(caddr);
		bzero(rbuf, sizeof(rbuf));
		recvfrom(udpfd, rbuf, sizeof(rbuf), 0, (struct sockaddr*)&caddr, &caddrlen);
#if 0
		bcopy(rbuf, sbuf, strlen(rbuf));
		bzero(sbuf + strlen(rbuf), sizeof(sbuf) - strlen(rbuf));
		sendto(udpfd, sbuf, strlen(sbuf), 0, (struct sockaddr*)&caddr, caddrlen);
#endif
	}
	close(udpfd);
	return 0;
}
