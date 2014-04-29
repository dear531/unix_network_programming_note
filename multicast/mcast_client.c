#include <stdio.h>

#include "share.h"
int setsockopt_func(int sockfd, int level, int optname,
	     const void *optval, socklen_t optlen)
{
	int ret;
	if ((ret = setsockopt(sockfd, level, optname, optval, optlen)) < 0)
	{
		fprintf(stdout, "setsockopt error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}

int main(int argc, char *argv[])
{
	/* create socket */
	int fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	/* bind local addr */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(MULTI_CAST_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	int opt = 25;
#if 0
	/* set time to leavel */
	fprintf(stdout, "set ttl\n");
	setsockopt_func(fd, IPPROTO_IP, IP_MULTICAST_TTL, &opt, sizeof(opt));
#endif
	/* broad loop local */
	fprintf(stdout, "set loop\n");
	opt = 1;
	setsockopt_func(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &opt, sizeof(opt));
	/* add multicast group */
	fprintf(stdout, "set add group\n");
	struct ip_mreq maddr;
	inet_pton_func(AF_INET, MULIT_CAST_IP, &maddr.imr_multiaddr);
	maddr.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt_func(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &maddr, sizeof(maddr));
	/* received and send data */
	char buf[1024];
	bzero(buf, sizeof(buf));
	socklen_t len;
	for ( ; ; )
	{
		buf[0] = '0';
		len = sizeof(saddr);
		recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&saddr, &len);
		sleep(1);
		sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&saddr, sizeof(saddr));
		fprintf(stdout, "sendto %s\n", buf);
	}
	fprintf(stdout, "leavel the group\n");
	/* leavel multicast group */
	setsockopt_func(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &maddr, sizeof(maddr));
	/* close socket */
	close(fd);
	return 0;
}
