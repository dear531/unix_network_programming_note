#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <linux/if.h>

int main(int argc, char *argv[])
{
	struct ifaddrs *ifap;
	int ret;
	if ((ret = getifaddrs(&ifap)) < 0)
	{
		fprintf(stdout, "get interface failed\n");
		exit(EXIT_FAILURE);
	}
	struct ifaddrs *tpi;
#define IPV4NUM		(3 * (4 + 1) - 1)
#define IPV6NUM		((128 / 16) * (4 + 1) - 1)
#define MAXIPNUM	(IPV4NUM > IPV6NUM ? IPV4NUM : IPV6NUM)
	char ip[MAXIPNUM + 1];
	char mask[MAXIPNUM + 1];
	char *p;
	for (tpi = ifap; tpi != NULL; tpi = tpi->ifa_next)
	{
		fprintf(stdout, "======\n");
		fprintf(stdout, "tpi->ifa_flags:%d\n", tpi->ifa_flags);
		fprintf(stdout, "name :%s\n", tpi->ifa_name);
		if (tpi->ifa_addr == NULL)
			continue;
		p = 	(tpi->ifa_addr->sa_family == AF_INET)
			? ((char *)&((struct sockaddr_in *)tpi->ifa_addr)->sin_addr)
			: (tpi->ifa_addr->sa_family == AF_INET6)
			? (char *)&(((struct sockaddr_in6 *)tpi->ifa_addr)->sin6_addr)
			: "";
		bzero(ip, sizeof(ip));
		inet_ntop(tpi->ifa_addr->sa_family, p, ip, sizeof(ip));
		fprintf(stdout, "ip :%s\n", ip);
		if (tpi->ifa_netmask == NULL)
			continue;
		p =	((tpi->ifa_netmask->sa_family == AF_INET)
			? (char *)&(((struct sockaddr_in *)tpi->ifa_netmask)->sin_addr)
			: (tpi->ifa_netmask->sa_family == AF_INET6)
			? (char *)&(((struct sockaddr_in6 *)tpi->ifa_netmask)->sin6_addr)
			: "");
		bzero(ip, sizeof(ip));
		inet_ntop(tpi->ifa_addr->sa_family, p, ip, sizeof(ip));
		fprintf(stdout, "mask :%s\n", ip);
#if 1
		if (tpi->ifa_flags & IFF_BROADCAST != 0)
		{
			if (tpi->ifa_broadaddr == NULL)
				continue;
			p =	((tpi->ifa_broadaddr->sa_family == AF_INET)
				? (char *)&(((struct sockaddr_in *)tpi->ifa_broadaddr)->sin_addr)
				: (tpi->ifa_broadaddr->sa_family == AF_INET6)
				? (char *)&(((struct sockaddr_in6 *)tpi->ifa_broadaddr)->sin6_addr)
				: "");
			bzero(ip, sizeof(ip));
			fprintf(stdout, "broadaddr: %s\n", inet_ntop(tpi->ifa_addr->sa_family, p, ip, sizeof(ip)));
		}
		else if (tpi->ifa_flags & IFF_POINTOPOINT != 0)
		{
			if (tpi->ifa_dstaddr == NULL)
				continue;
			p =	((tpi->ifa_dstaddr->sa_family == AF_INET)
				? (char *)&(((struct sockaddr_in *)tpi->ifa_dstaddr)->sin_addr)
				: (tpi->ifa_dstaddr->sa_family == AF_INET6)
				? (char *)&(((struct sockaddr_in6 *)tpi->ifa_dstaddr)->sin6_addr)
				: "");
			bzero(ip, sizeof(ip));
			fprintf(stdout, "dstaddr: %s\n", inet_ntop(tpi->ifa_addr->sa_family, p, ip, sizeof(ip)));
		}
#endif
	}




#if 0
	struct ifaddrs {
		struct ifaddrs  *ifa_next;    /* Next item in list */
		char            *ifa_name;    /* Name of interface */
		unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */
		struct sockaddr *ifa_addr;    /* Address of interface */
		struct sockaddr *ifa_netmask; /* Netmask of interface */
		union {
			struct sockaddr *ifu_broadaddr;
			/* Broadcast address of interface */
			struct sockaddr *ifu_dstaddr;
			/* Point-to-point destination address */
		} ifa_ifu;
#define              ifa_broadaddr ifa_ifu.ifu_broadaddr
#define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
		void            *ifa_data;    /* Address-specific data */
	};

#endif
	freeifaddrs(ifap);

	return 0;
}
