#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>



int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
	{
		fprintf(stdout, "socket failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
int ioctl_func(int fd, int request, void *argp)
{
	int ret;
	if ((ret = ioctl(fd, request, argp)) < 0)
	{
		fprintf(stdout, "ioctl failure :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}

int main(int argc, char *argv[])
{
	/* create socket */
	int	fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* successivily get interface infomation */
	struct arpreq	arpp;
	char ip[(128 / 16) * (4 + 1)];
	/* arp commond resolu query link ip :192.168.1.2 */
#if 0
	inet_pton(AF_INET, "192.168.1.2", &((struct sockaddr_in*)&arpp.arp_pa)->sin_addr);
#else
	if (argc != 3)
	{
		fprintf(stdout, "usage : ./a.out <ipaddr> <netdevice>\n");
		exit(EXIT_FAILURE);
	}
	inet_pton(AF_INET, argv[1], &((struct sockaddr_in*)&arpp.arp_pa)->sin_addr);
#endif
	bzero(arpp.arp_dev, sizeof(arpp.arp_dev));
	arpp.arp_pa.sa_family = AF_INET;
	/* ip :192.168.1.2 corresponding netdevice name */
#if 0
	bcopy("eth0", arpp.arp_dev, strlen("eth0"));
#else
	bcopy(argv[2], arpp.arp_dev, strlen(argv[2]));
#endif
	ioctl_func(fd, SIOCGARP, &arpp);
	int i;	
	for (i = 0; i < 6; i++)
	{
		if (i == 0)
			fprintf(stdout, "arp_ha:");
		fprintf(stdout, "%02x", (unsigned char)arpp.arp_ha.sa_data[i]);
		if (i == 5)
			fprintf(stdout, "\n");
		else
			fprintf(stdout, ":");
	}
	/* free */

	/* close socket*/
	close(fd);
	return 0;
}
