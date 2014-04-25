#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
	{
		fprintf(stdout, "scoekt failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int ioctl_func(int d, int request, void *p)
{
	int ret;
	if ((ret = ioctl(d, request, p)) < 0)
	{
		fprintf(stdout, "ioctl failure :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
void *realloc_func(void *ptr, size_t size)
{
	void *p;
	if ((p = realloc(ptr, size)) < 0)
	{
		fprintf(stdout, "malloc failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return p;
}
int main(int argc, char *argv[])
{
	int fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct ifconf	 ifcp;
	struct ifreq	*ifrp;
	int num;
	int n = 0;
	for (ifrp = NULL, num = 1; ; num *= 2)
	{
		ifrp = realloc_func(ifrp, sizeof(*ifrp) * num);
		ifcp.ifc_len 	= sizeof(*ifrp) * num;
		ifcp.ifc_req	= ifrp;
		ioctl_func(fd, SIOCGIFCONF, &ifcp);
		if (ifcp.ifc_len < sizeof(*ifrp) * num)
		{
			ioctl_func(fd, SIOCGIFCONF, &ifcp);
			ifcp.ifc_req	= ifrp;
			break;
		}
	}
	int len = 0;
	len = ifcp.ifc_len / sizeof(*ifrp);
	int i;
	char ipaddr[(128 / 16) * (4 + 1)];
	int j;
	for (i = 0; i < len; i++)
	{
		fprintf(stdout, "name[%d]\t:%s\n", i, ifrp[i].ifr_name);
		if (ioctl(fd, SIOCGIFFLAGS, ifrp + i))
			continue;
		fprintf(stdout, "flags\t:%d\n", ifrp[i].ifr_flags);
		ioctl_func(fd, SIOCGIFADDR, ifrp + i);
		fprintf(stdout, "addr\t:%s\n", inet_ntop(AF_INET, &((struct sockaddr_in *)&(ifrp[i].ifr_addr))->sin_addr, ipaddr, sizeof(ipaddr)));
		ioctl_func(fd, SIOCGIFFLAGS, ifrp + i);
		if (ifrp[i].ifr_flags & IFF_BROADCAST != 0)
		{
			ioctl_func(fd, SIOCGIFBRDADDR, ifrp + i);
			fprintf(stdout, "broadaddr:%s\n", inet_ntop(AF_INET, &((struct sockaddr_in *)&(ifrp[i].ifr_broadaddr))->sin_addr, ipaddr, sizeof(ipaddr)));
		}
		ioctl_func(fd, SIOCGIFNETMASK, ifrp + i);
		fprintf(stdout, "netmask\t:%s\n", inet_ntop(AF_INET, &((struct sockaddr_in *)&(ifrp[i].ifr_netmask))->sin_addr, ipaddr, sizeof(ipaddr)));
		ioctl_func(fd, SIOCGIFHWADDR, ifrp + i);
		for (j = 0; j < 7; j++)
		{
			if (j == 0)
				fprintf(stdout, "mac\t:");
			fprintf(stdout, "%02x", (unsigned char)ifrp[i].ifr_hwaddr.sa_data[j]);
			if (j == 6)
				putchar('\n');
			else
				fprintf(stdout, ":");
		}
		ioctl_func(fd, SIOCGIFMTU, ifrp + i);
		fprintf(stdout, "ifr_mtu\t:%d\n", ifrp[i].ifr_mtu);
		
		putchar('\n');
	}
	/* man 7 netdevice */
#if 0
			struct ifreq {
				char ifr_name[IFNAMSIZ]; /* Interface name */
				union {
					struct sockaddr ifr_addr;
					struct sockaddr ifr_dstaddr;
					struct sockaddr ifr_broadaddr;
					struct sockaddr ifr_netmask;
					struct sockaddr ifr_hwaddr;
					short           ifr_flags;
					int             ifr_ifindex;
					int             ifr_metric;
					int             ifr_mtu;
					struct ifmap    ifr_map;
					char            ifr_slave[IFNAMSIZ];
					char            ifr_newname[IFNAMSIZ];
					char           *ifr_data;
				};
			};

		struct ifconf {
			int                 ifc_len; /* size of buffer */
			union {
				char           *ifc_buf; /* buffer address */
				struct ifreq   *ifc_req; /* array of structures */
			};
		};

#endif
	free(ifrp);
	close(fd);

	return 0;
}
