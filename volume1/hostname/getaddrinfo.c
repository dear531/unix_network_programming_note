#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <mcheck.h>  
#include <arpa/inet.h>

int
main(int argc, char *argv[])
{
	mtrace();
	char *psname;
	char *psserv;
	struct addrinfo ssetai, *pretai, *pnext, *pcrnt;
	int ret;
	bzero(&ssetai, sizeof(ssetai));
	/*
	 * ai_canonname field of first of the addrinfo structures in the returned list
	 * is set to point to the official name of the host
	 */
	ssetai.ai_flags |= AI_CANONNAME;
#if 0	/* normal */
	psname = "baidu.com";
	psserv = "http";
#elif 0	/* AI_NUMERICHOST : ipput suppresses hostname, must ip address */
	ssetai.ai_flags |= AI_NUMERICHOST;
	/* psname = "hao123.com"; will be error :Name or service not known */
	psname = "127.0.0.1";
	psserv = "http";
#elif 0	/* AI_PASSIVE : returned wildcard  addres in res */
	/* psname = "127.0.0.1"; If node is not NULL, then the AI_PASSIVE flag is ignored */
	psname = NULL;
	psserv = "http";
	ssetai.ai_flags |= AI_PASSIVE;
#elif 0
	psname = "127.0.0.1";
	/* If service  is  NULL, then port number uninitialized */
	psserv = NULL;
#elif 0	/* can be service port by decimal number */
	psname = "127.0.0.1";
	psserv = "80";
#elif 0	/* AI_NUMERICSERV : must be service by decimal number */
	ssetai.ai_flags |= AI_NUMERICSERV;
	psname = "127.0.0.1";
	psserv = "80";
	/* psserv = "http"; will be error :Name or service not known */
#elif 0 /* AI_ADDRCONFIG : least local ip */
	ssetai.ai_flags |= AI_ADDRCONFIG;
	psname = "zly-ThinkPad-Edge-E430";
	psserv = "http";
#elif 0 /* AF_UNSPEC : Allow IPv4 or IPv6 */
	ssetai.ai_family = AF_UNSPEC;
	psname = "zly-ThinkPad-Edge-E430";
	psserv = "http";
#elif 0
	ssetai.ai_flags |= AI_V4MAPPED | AI_ALL;
	ssetai.ai_family = AF_INET6;
	psname = "fe80::21e:ecff:fe8e:1af4";
	psserv = "http";
#elif 1
	ssetai.ai_flags |= AI_V4MAPPED | AI_ALL;
	ssetai.ai_family = AF_INET6;
	psname = "fe80::21e:ecff:fe8e:1af4";
	/* psname = "zly-ThinkPad-Edge-E430"; show:addr:::ffff:127.0.1.1*/
	psserv = "http";
#endif
	ret = getaddrinfo(psname, psserv, &ssetai, &pretai);
	if (ret < 0)
	{
		fprintf(stdout, "getaddrinfo error :%s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
#if 0
	struct addrinfo {
		int              ai_flags;
		int              ai_family;
		int              ai_socktype;
		int              ai_protocol;
		size_t           ai_addrlen;
		struct sockaddr *ai_addr;
		char            *ai_canonname;
		struct addrinfo *ai_next;
	};
	struct sockaddr_in {
		sa_family_t    sin_family; /* address family: AF_INET */
		in_port_t      sin_port;   /* port in network byte order */
		struct in_addr sin_addr;   /* internet address */
	};

	/* Internet address. */
	struct in_addr {
		uint32_t       s_addr;     /* address in network byte order */
	};

#endif
	char *pfml	= NULL;
	char *pprtcl	= NULL;
	char *pscktp	= NULL;
	for (pnext = pretai; pnext != NULL; pnext = pnext->ai_next)
	{
		fprintf(stdout, "ai_flags:%d\n", pnext->ai_flags);
		switch (pnext->ai_family)
		{
			case AF_INET:	pfml = "AF_INET"; break; 
			case AF_INET6:	pfml = "AF_INET6";break;
		}
		fprintf(stdout, "ai_family:%s\n", pfml);
		fprintf(stdout, "ai_canonname :%s\n", pnext->ai_canonname);
		char buf[128];
		bzero(buf, sizeof(buf));
		if (pnext->ai_family == AF_INET)
			fprintf(stdout, "ai_addr->addr:%s\n", inet_ntoa(((struct sockaddr_in*)pnext->ai_addr)->sin_addr));
		else if (pnext->ai_family == AF_INET6)
			fprintf(stdout, "ai_addr->addr:%s\n",
				inet_ntop(AF_INET6, &((struct sockaddr_in6*)pnext->ai_addr)->sin6_addr,
				buf, sizeof(buf)));
		fprintf(stdout, "ai_addr->port:%d\n", htons(((struct sockaddr_in*)pnext->ai_addr)->sin_port));
		switch (pnext->ai_socktype)
		{
			case SOCK_STREAM:	pscktp = "SOCK_STREAM"; break;
			case SOCK_DGRAM:	pscktp = "SOCK_DGRAM";break;
			default:;
		}
		fprintf(stdout, "ai_addr->socktype:%s\n", pscktp);
		switch (pnext->ai_protocol)
		{
			case IPPROTO_TCP: pprtcl = "IPPROTO_TCP"; break;
			case IPPROTO_UDP: pprtcl = "IPPROTO_UDP"; break;
			default:;
		}
		fprintf(stdout, "ai_addr->protocol:%s\n", pprtcl);
		fprintf(stdout, "pnext :%p, pnext->ai_next:%p\n", pnext, pnext->ai_next);
		sleep(1);
	}
	/* to be use mtrace checked, freeaddrinfo free all neto on linked list */
	/*
	 * does not freeaddrinfo function show: 
	 *
	 * $ mtrace a.out a.log
	 * 
	 * Memory not freed:
	 * -----------------
	 *            Address     Size     Caller
	 * 0x0000000000792dc0      0xb  at 0x7fc718c6ed82
	 * 0x0000000000792de0     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792e30     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792e80     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792ed0     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792f20     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792f70     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000792fc0     0x40  at 0x7fc718cb1aa4
	 * 0x0000000000793010     0x40  at 0x7fc718cb1aa4
	 *
	 * called freeaddrinfo function show:
	 * $ mtrace a.out a.log
	 * No memory leaks.
	 *
	 */
	freeaddrinfo(pretai);

	return 0;
}
