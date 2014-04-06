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
#define UDP_CONNECT	1
#if UDP_CONNECT
	int ret;
	ret = connect(udpfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret < 0)
		fprintf(stdout, "connect server error: %s\n", strerror(errno));
#endif
	struct sockaddr_in laddr, paddr;
	socklen_t laddrlen = sizeof(laddr), paddrlen = sizeof(paddr);
	char lipbuf[16], pipbuf[16];
	int i;
	for (i = 0; i < 1024; i++)
		sbuf[i] = '0' + i % 10;
	for (i = 0; i < 2000; i++)
	{
#if UDP_CONNECT
		n = send(udpfd, sbuf, strlen(sbuf), 0);
		//n = write(udpfd, sbuf, strlen(sbuf));
		if (n < 0)
		{
			fprintf(stdout, "send error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "send sbuf :%s\n", sbuf);
		
		//recv(udpfd, rbuf, sizeof(rbuf), 0);

#else
		//fprintf(stdout, "input sbuf :%s\n", sbuf);
		sendto(udpfd, sbuf, strlen(sbuf), 0, (struct sockaddr*)&saddr, sizeof(saddr));
		//fprintf(stdout, "isnot?!\n");
# if 0
		bzero(rbuf, sizeof(rbuf));
		saddrlen = sizeof(saddr);
		bzero(pcmpsaddr, sizeof(saddr));
		n = recvfrom(udpfd, rbuf, sizeof(rbuf), 0, pcmpsaddr, &saddrlen);
		if (n < 0)
			fprintf(stdout, "udp recv error: %s\n", strerror(errno));
		if (saddrlen == sizeof(saddr) && bcmp(pcmpsaddr, &saddr, saddrlen) == 0)
			fprintf(stdout, "received buf :%s strlen %d\n", rbuf, n);
# endif
#endif
		getsockname(udpfd, (struct sockaddr*)&laddr, &laddrlen);
		bzero(lipbuf, sizeof(lipbuf));
		inet_ntop(AF_INET, &laddr.sin_addr, lipbuf);
		fprintf(stdout, "load host addr:%s, port:%d\n", lipbuf, ntohs(laddr.sin_port));

		getpeername(udpfd, (struct sockaddr*)&paddr, &paddrlen);
		bzero(pipbuf, sizeof(pipbuf));
		inet_ntop(AF_INET, &paddr.sin_addr, pipbuf);
		fprintf(stdout, "peer host addr:%s, port:%d\n", pipbuf, ntohs(paddr.sin_port));
		sleep(1);
		if (i == 20)
		{
			bzero(&saddr, sizeof(saddr));
			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(8000);
			inet_pton(AF_INET, "192.168.1.111", &saddr.sin_addr);
			ret = connect(udpfd, (struct sockaddr*)&saddr, sizeof(saddr));
			if (ret < 0)
				fprintf(stdout, "connect server error: %s\n", strerror(errno));
		}
	}
	sleep(100);
	fprintf(stdout, "i :%d\n", i);
	free(pcmpsaddr);
	pcmpsaddr = NULL;
	close(udpfd);
	udpfd = -1;
	return;
}
