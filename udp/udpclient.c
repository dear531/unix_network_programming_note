#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int udpfd;
	udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	char sbuf[1024], rbuf[1024];
	struct sockaddr_in saddr;
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	int n;
	int saddrlen;
	struct sockaddr * pcmpsaddr;
	pcmpsaddr = malloc(sizeof(saddr));
	while(bzero(sbuf, sizeof(sbuf)), (gets(sbuf)) != NULL)
	{
		fprintf(stdout, "input sbuf :%s\n", sbuf);
		sendto(udpfd, sbuf, strlen(sbuf), 0, (struct sockaddr*)&saddr, sizeof(saddr));
		bzero(rbuf, sizeof(rbuf));
		saddrlen = sizeof(saddr);
		bzero(pcmpsaddr, sizeof(saddr));
		n = recvfrom(udpfd, rbuf, sizeof(rbuf), 0, pcmpsaddr, &saddrlen);
		if (n < 0)
			fprintf(stdout, "udp recv error: %s\n", strerror(errno));
		if (saddrlen == sizeof(saddr) && bcmp(pcmpsaddr, &saddr, saddrlen) == 0)
			fprintf(stdout, "received buf :%s strlen %d\n", rbuf, n);
	}
	free(pcmpsaddr);
	close(udpfd);
	return;
}
