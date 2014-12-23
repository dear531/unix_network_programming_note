#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
	/* get addr info  */
	struct addrinfo ss, *rp, *rtp;
#if 0
	struct addrinfo {
		int              ai_flags;
		int              ai_family;
		int              ai_socktype;
		int              ai_protocol;
		socklen_t        ai_addrlen;
		struct sockaddr *ai_addr;
		char            *ai_canonname;
		struct addrinfo *ai_next;
	};
#endif
	bzero(&ss, sizeof(ss));
	ss.ai_flags	= AI_PASSIVE | AI_CANONNAME;
	ss.ai_family	= AF_UNSPEC;
	int ret;
	int fd;
	ret = getaddrinfo(argv[1], argv[2], &ss, &rp);
	if (ret < 0)
	{
		fprintf(stdout, "get address infomation error :%s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
	for (rtp = rp; rtp != NULL; rtp = rtp->ai_next)
	{
		fprintf(stdout, "ai_canonname :%s\n", rtp->ai_canonname);
		/* socket */
		if (rtp->ai_socktype != SOCK_DGRAM || rtp->ai_protocol != IPPROTO_UDP)
			continue;
		fd = socket(rtp->ai_family, rtp->ai_socktype, rtp->ai_protocol);
		if (fd < 0)
		{
			fprintf(stdout, "socket error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		/* bind */
		ret = bind(fd, (struct sockaddr *)rtp->ai_addr, rtp->ai_addrlen);
		if (ret < 0)
		{
			fprintf(stdout, "bind error :%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else
		{
			break;
		}
	}
	/* freeaddrinfo */
	freeaddrinfo(rtp);
	struct sockaddr caddr;
	char sbuf[1024];
	int slen = sizeof(caddr);
	while (1)
	{
		/* wait rece from */
		bzero(sbuf, sizeof(sbuf));
		recvfrom(fd, sbuf, sizeof(sbuf), 0, &caddr, &slen); 
		fprintf(stdout, "http for udp received :%s\n", sbuf);
		/* sendto */
		sendto(fd, sbuf, strlen(sbuf), 0, &caddr, slen); 
	}
	/* close */
	return 0;
}
