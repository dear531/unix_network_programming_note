#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int udpfd;
	udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#if 0
           struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };

#endif
	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(udpfd, &saddr.sin_addr, sizeof(saddr));
	bind(udpfd, (struct sockaddr*)&saddr, sizeof(saddr));
	int caddrlen;
	char rbuf[1024], sbuf[1024];
	while(1)
	{
		caddrlen = sizeof(caddr);
		bzero(rbuf, sizeof(rbuf));
		recvfrom(udpfd, rbuf, sizeof(rbuf), 0, (struct sockaddr*)&caddr, &caddrlen);

		bcopy(rbuf, sbuf, strlen(rbuf));
		bzero(sbuf + strlen(rbuf), sizeof(sbuf) - strlen(rbuf));
		sendto(udpfd, sbuf, strlen(sbuf), 0, (struct sockaddr*)&caddr, caddrlen);
	}
	close(udpfd);
	return 0;
}
